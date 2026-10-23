/*
 * A159806  Numerator of Hermite(n, 1/22).
 * https://oeis.org/A159806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = a(n-1) - 242*(n-1)*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from a(n) = 11^n * Hermite(n, 1/22) and the recurrence for Hermite polynomials. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)1;
    seq_t a_nm2 = (seq_t)1;       /* a(0) */
    seq_t a_nm1 = (seq_t)1;       /* a(1) */
    seq_t a_n   = (seq_t)0;       /* placeholder */
    for (idx_t i = 2ULL; i <= n; ++i)
    {
        seq_t c = ((seq_t)242) * (seq_t)(i - 1ULL); /* coefficient 242*(i-1) */
        a_n = a_nm1 - c * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
