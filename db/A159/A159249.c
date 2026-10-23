/*
 * A159249  Numerator of Hermite(n, 3/10).
 * https://oeis.org/A159249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5^n * Hermite(n, 3/10). Recurrence: a(0) = 1, a(1) = 3,
 *            a(n+1) = 3*a(n) - 50*n*a(n-1) for n >= 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;          /* a(0) */
    seq_t a_nm1 = (seq_t)3;          /* a(1) */
    seq_t a_n   = 0;                 /* placeholder */

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* a(i) = 3*a(i-1) - 50*(i-1)*a(i-2) */
        a_n = (seq_t)3 * a_nm1
            - (seq_t)50 * ( (seq_t)(i - 1u) ) * a_nm2;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
