/*
 * A159869  Numerator of Hermite(n, 5/23).
 * https://oeis.org/A159869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 10*a(n-1) + 1058*(1-n)*a(n-2), Initial values: a(0)=1,
 *            a(1)=10
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)10;  /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t one_minus_i = (seq_t)1 - (seq_t)i;          /* 1 - i */
        a_n = (seq_t)10 * a_nm1 + (seq_t)1058 * one_minus_i * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
