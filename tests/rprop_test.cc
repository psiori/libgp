// libgp - Gaussian process library for Machine Learning
// Copyright (c) 2011, Manuel Blum <mblum@informatik.uni-freiburg.de>
// All rights reserved.

#include "gp.h"
#include "rprop.h"
#include "gp_utils.h"

#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

TEST(RPropTest, Test1) 
{
  int input_dim = 3, param_dim = 3;
  libgp::GaussianProcess * gp = new libgp::GaussianProcess(input_dim, "CovSum ( CovSEiso, CovNoise)");
  Eigen::VectorXd params(param_dim);
  params << 0, 0, log(0.01);
  gp->covf().set_loghyper(params);
  int n = 500;
  Eigen::MatrixXd X(n, input_dim);
  X.setRandom();
  X = X*10;
  Eigen::VectorXd y = gp->covf().draw_random_sample(X);
  for(size_t i = 0; i < n; ++i) {
    double x[input_dim];
    for(int j = 0; j < input_dim; ++j) x[j] = X(i,j);
    gp->add_pattern(x, y(i));
  }

  params << -1, -1, -1;
  gp->covf().set_loghyper(params);
  
  libgp::RProp rprop;
  rprop.init();
  rprop.maximize(gp, 50, 0);

  ASSERT_NEAR(0, gp->covf().get_loghyper()(0), 0.1);
  ASSERT_NEAR(0, gp->covf().get_loghyper()(1), 0.1);

}


