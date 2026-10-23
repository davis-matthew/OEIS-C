/*
 * A016208  Expansion of 1/((1-x)*(1-3*x)*(1-4*x)).
 * https://oeis.org/A016208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 19*a(n-2) + 12*a(n-3); Initial values: a(0)=1,
 *            a(1)=8, a(2)=45
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)45;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)8;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)45;  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = 8*a(n-1) - 19*a(n-2) + 12*a(n-3) */
        a_n = 8 * a_nm1 + 12 * a_nm3; /* 8*a(n-1) + 12*a(n-3) */
        a_n -= 19 * a_nm2;           /* subtract 19*a(n-2) */
        a_nm3 = a_nm2;   /* shift indices for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
