/*
 * A159707  Numerator of Hermite(n, 4/21).
 * https://oeis.org/A159707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1; a(1)=8; a(n)=8*a(n-1)-882*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* For uint64_t if needed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a_nm2 = (seq_t)1;           /* a(0) */
    seq_t a_nm1 = (seq_t)8;           /* a(1) */
    seq_t a_n   = (seq_t)0;           /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* multiplier = 882 * (i-1), computed as a 64‑bit integer then cast */
        seq_t multiplier = (seq_t)(882 * (i - 1));
        a_n = (seq_t)8 * a_nm1 - multiplier * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
