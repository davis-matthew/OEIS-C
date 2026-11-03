/*
 * A005807  Sum of adjacent Catalan numbers.
 * https://oeis.org/A005807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) + C(n+1), with Catalan C(0) = 1, C(k+1) =
 *            C(k)*2*(2k+1)/(k+2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 184 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* C(k+1) from c = C(k), exactly, keeping intermediates no larger than the result */
static seq_t a005807_next(seq_t c, idx_t k) {
    uint64_t d = (uint64_t)k + 2;
    uint64_t m = 2 * (2 * (uint64_t)k + 1);
    uint64_t r = (uint64_t)(c % (seq_t)d);
    uint64_t g = gcd_u64(d, r);
    c /= (seq_t)g;
    d /= g;
    /* gcd(c, d) = 1 now and d divides c*m, so d divides m */
    return c * (seq_t)(m / d);
}

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 0; k < n; k++)
        c = a005807_next(c, k);
    return c + a005807_next(c, n);
}
