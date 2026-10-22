/*
 * A001500  Number of stochastic matrices of integers: n X n arrays of
 *          nonnegative integers with all row and column sums equal to 3.
 * https://oeis.org/A001500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(-n) * Sum_{alpha=0..n, beta=0..n-alpha}
 *            2^alpha*3^beta*(n!)^2*(3n-3alpha-2beta)! /
 *            (alpha!*beta!*(n-alpha-beta)!^2*6^(n-alpha-beta)). With g =
 *            n-alpha-beta, m = n-g: a(n) = 6^(-2n) * Sum_{g=0..n}
 *            C(n,g)*(n!/g!)*6^m * I(g), I(g) = Sum_{b=0..m}
 *            C(m,b)*2^(m-b)*3^b*(3g+b)!. Fixed-width buil...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   a001500_big, a001500_dp, a001500_fixedw, a001500_sb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001500_big.h"
#include "../utils/a001500_dp.h"
#include "../utils/a001500_fixedw.h"
#include "../utils/a001500_sb.h"

seq_t a(idx_t n) {
    if (a001500_fixedw()) {
        if (n > 64) return 0; /* far beyond any fixed width */
        return a001500_dp(n);
    }
    return a001500_big(n);
}
