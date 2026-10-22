/*
 * A001755  Lah numbers: a(n) = n! * binomial(n-1, 3)/4!.
 * https://oeis.org/A001755
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * n*(n-1)/(n-4), a(4) = 1 (i.e. a(n) = n! *
 *            binomial(n-1, 3)/4!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   lah4_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lah4_gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 5; k <= n; k++) {
        idx_t m = k * (k - 1);
        idx_t d = k - 4;
        idx_t g = lah4_gcd(m, d);
        m /= g;
        d /= g;
        /* gcd(m, d) = 1 and a(k) is an integer, so d divides a(k-1) */
        result = result / (seq_t)d * (seq_t)m;
    }
    return result;
}
