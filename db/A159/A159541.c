/*
 * A159541  Numerator of Hermite(n, 13/17).
 * https://oeis.org/A159541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 26*a(n-1) - 578*(n-1)*a(n-2) for n >= 2, a(0) = 1, a(1) =
 *            26. This follows from the two‑term recurrence for 17ⁿ·Hₙ(13/17)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)26;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 578 * (i-1) * a_nm2 */
        seq_t term = (seq_t)578 * (seq_t)(i-1) * a_nm2;
        cur = (seq_t)26 * a_nm1 - term;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
