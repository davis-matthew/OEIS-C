/*
 * A006281  Partial quotients in continued fraction expansion of 2C-1, where C
 *          is Cahen's constant.
 * https://oeis.org/A006281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 3, and for n >= 2, a(n) = 2*r(n-2)^2, where
 *            r(0) = 1, r(1) = 3, r(k) = 2*r(k-2)^2*r(k-1) + r(k-2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    idx_t m = n - 2;
    seq_t r0 = 1, r1 = 3;
    if (m == 0) return (seq_t)2 * r0 * r0;
    for (idx_t k = 2; k <= m; k++) {
        seq_t r2 = (seq_t)2 * r0 * r0 * r1 + r0;
        r0 = r1;
        r1 = r2;
    }
    return (seq_t)2 * r1 * r1;
}
