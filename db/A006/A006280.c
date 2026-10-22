/*
 * A006280  Partial quotients in continued fraction expansion of Cahen's
 *          constant.
 * https://oeis.org/A006280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A006279(n-2)^2 for n > 1, with a(0) = 0, a(1) = 1; q(0) =
 *            q(1) = 1, q(i) = a(i)*q(i-1) + q(i-2), a(i) = q(i-2)^2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t q2, q1, c, qi;
    idx_t i;

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    q2 = (seq_t)1;   /* q(i-2) */
    q1 = (seq_t)1;   /* q(i-1) */
    c = (seq_t)1;
    for (i = 2; i <= n; i++) {
        c = q2 * q2;
        if (i == n) break;
        qi = c * q1 + q2;
        q2 = q1;
        q1 = qi;
    }
    return c;
}
