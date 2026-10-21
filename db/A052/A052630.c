/*
 * A052630  E.g.f. 1/(1-4x-x^2).
 * https://oeis.org/A052630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4
 * a(n+2) = (2 + n^2 + 3*n) * a(n) + (4*n + 8) * a(n+1)  for n ≥ 0
 * This implements the sequence A052630 whose egf is 1/(1-4x-x²). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a_nm1 = (seq_t)1;   /* a(i)   = a_{i}   */
    seq_t a_n   = (seq_t)4;   /* a(i+1) = a_{i+1} */

    for (idx_t i = 0; i < n - 1; ++i)
    {
        /* coefficients for a(i) and a(i+1) */
        seq_t i_seq       = (seq_t)i;
        seq_t coeff1      = (seq_t)2 + i_seq * i_seq + (seq_t)3 * i_seq;
        seq_t coeff2      = (seq_t)4 * i_seq + (seq_t)8;

        /* compute a(i+2) = coeff1*a(i) + coeff2*a(i+1) */
        seq_t next = coeff1 * a_nm1 + coeff2 * a_n;

        /* shift values for next iteration */
        a_nm1 = a_n;
        a_n   = next;
    }

    return a_n;   /* a_n now holds a(n) */
}
