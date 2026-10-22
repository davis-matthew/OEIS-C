/*
 * A005516  Number of n-bead bracelets (turnover necklaces) with 12 red beads.
 * https://oeis.org/A005516
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. x^12*((1/12)*(Sum_{d|12} phi(d)/(1-x^d)^(12/d)) +
 *            (1+x)/(1-x^2)^7)/2, so with m = n-12: a(n) = ( C(m+11,11) +
 *            [2|m]C(m/2+5,5) + 2[3|m]C(m/3+3,3) + 2[4|m]C(m/4+2,2) +
 *            2[6|m]C(m/6+1,1) + 4[12|m] + 12*C(floor(m/2)+6,6) ) / 24, each
 *            term divided by 24 separately (quotient + remainder) to avoid
 *            o...
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 55, u64 n <= 375,
 *            u128 n <= 1000, big any n
 * Helpers:   acc_div24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/acc_div24.h"

seq_t a(idx_t n) {
    seq_t Q = 0;
    uint64_t R = 0;
    uint64_t m;
    if (n < 12) return 0;
    m = (uint64_t)n - 12;

    acc_div24(m + 11, 11, 1, &Q, &R);
    if (m % 2 == 0) acc_div24(m / 2 + 5, 5, 1, &Q, &R);
    if (m % 3 == 0) acc_div24(m / 3 + 3, 3, 2, &Q, &R);
    if (m % 4 == 0) acc_div24(m / 4 + 2, 2, 2, &Q, &R);
    if (m % 6 == 0) acc_div24(m / 6 + 1, 1, 2, &Q, &R);
    if (m % 12 == 0) R += 4;
    acc_div24(m / 2 + 6, 6, 12, &Q, &R);

    Q += (seq_t)(R / 24);
    return Q;
}
