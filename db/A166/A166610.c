/*
 * A166610  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1, a(1) = 23,
 * a(n) = 22*a(n-1)                         for 2 ≤ n ≤ 11,
 * a(12) = 22*a(11) - 252*a(0) - 1,
 * a(13) = 22*a(12) - 252*a(1) + 231*a(0) - 1,
 * a(n) = 22*a(n-1) - 252*a(n-12) + 231*a(n-13)   for n ≥ 14
 * These relations follow from the rational generating function
 * (1+x)(1-x^12)/(1-22*x+252*x^12-231*x^13).
 */

#include <stddef.h>  /* for idx_t */

/* Public entry point.  External linkage is required. */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;

    /* Circular buffer for the last 14 computed values (indices modulo 14). */
    seq_t buf[14] = {0};
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)23;

    /* Compute a(2) .. a(n) iteratively. */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t val;
        idx_t idx_minus_1  = (i - 1) % 14;
        idx_t idx_minus_12 = (i >= 12)   ? (i - 12)   % 14 : 0;
        idx_t idx_minus_13 = (i >= 13)   ? (i - 13)   % 14 : 0;

        if (i < 12)
        {
            val = (seq_t)22 * buf[idx_minus_1];
        }
        else if (i == 12)
        {
            val = (seq_t)22 * buf[idx_minus_1] - (seq_t)252 * buf[0] - (seq_t)1;
        }
        else if (i == 13)
        {
            val = (seq_t)22 * buf[idx_minus_1]
                 - (seq_t)252 * buf[idx_minus_12]
                 + (seq_t)231 * buf[idx_minus_13]
                 - (seq_t)1;
        }
        else /* i >= 14 */
        {
            val = (seq_t)22 * buf[idx_minus_1]
                 - (seq_t)252 * buf[idx_minus_12]
                 + (seq_t)231 * buf[idx_minus_13];
        }
        buf[i % 14] = val;
    }

    return buf[n % 14];
}
