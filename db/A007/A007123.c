/*
 * A007123  Number of connected unit interval graphs with n nodes; also number
 *          of bracelets (turnover necklaces) with n black beads and n-1 white
 *          beads.
 * https://oeis.org/A007123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n+1) = (Catalan(n) + binomial(n, floor(n/2)))/2, with
 *            Catalan(k+1) = Catalan(k)*(4k+2)/(k+2), Catalan(0) = 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a007123_catalan, a007123_g, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/a007123_catalan.h"
#include "../utils/a007123_g.h"

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t c = a007123_catalan(m);
    seq_t b = binom(m, m / 2);
    return c / 2 + b / 2 + (c % 2 + b % 2) / 2;
}
