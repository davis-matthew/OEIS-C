/*
 * A001453  Catalan numbers - 1.
 * https://oeis.org/A001453
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000108(n) - 1, where C(0) = 1, C(k) =
 *            C(k-1)*2*(2k-1)/(k+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a001453_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001453_gcd.h"

seq_t a(idx_t n) {
    seq_t c = 1;
    for (idx_t k = 1; k <= n; k++) {
        idx_t m = 2 * (2 * k - 1);
        idx_t d = k + 1;
        idx_t g = a001453_gcd(m, d);
        m /= g;
        d /= g;
        /* c*m/d is an integer and gcd(m,d)=1, so d divides c */
        c = c / (seq_t)d * (seq_t)m;
    }
    return c - 1;
}
