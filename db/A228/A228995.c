/*
 * A228995  Total sum of the 5th powers of lengths of ascending runs in all
 *          permutations of [n].
 * https://oeis.org/A228995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(1) = 1 For n >= 2: a(n) = 2*n*a(n-1) - n*(n-1)*a(n-2)
 *            + 20*n*(n-1)*(n-2) + 30*n - 30
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = (seq_t)0;  /* a(0) */
    seq_t a_nm1 = (seq_t)1;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute each term of the recurrence using only seq_t arithmetic. */
        seq_t term1 = (seq_t)(2 * (seq_t)i) * a_nm1;                    /* 2*i*a(n-1)          */
        seq_t term2 = (seq_t)i * (seq_t)(i - 1) * a_nm2;                /* i*(i-1)*a(n-2)       */
        seq_t term3 = (seq_t)20 * (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2); /* 20*i*(i-1)*(i-2)  */
        seq_t term4 = (seq_t)30 * (seq_t)i - (seq_t)30;                /* 30*i - 30            */

        /* a(i) = term1 + term3 + term4 - term2   (term1 is always >= term2 for n>=2) */
        seq_t a_i = term1 + term3 + term4;
        a_i -= term2;

        /* Shift for next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;
}
