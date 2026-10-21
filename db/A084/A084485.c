/*
 * A084485  Number of 3 X n 0-1 matrices which have n+2 1's and have no zero
 *          rows or zero columns.
 * https://oeis.org/A084485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*(3*n-1)*3^n-9*(n-1)*2^n)/24
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   power_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(1) = 1, a(2) = 12, a(3) = 90, …   (OEIS A084485) */

#include <stdint.h>
#include "../utils/power_seq_t.h"

seq_t a(idx_t n)
{
    /* treat n as seq_t early to prevent signed/unsigned mixing      */
    seq_t n_seq = (seq_t)n;

    if (n_seq == (seq_t)0) {          /*  not used by the sequence, but safe */
        return (seq_t)0;
    }

    /* powers of 2 and 3                                         */
    seq_t pow2 = power_seq_t((seq_t)2, n);
    seq_t pow3 = power_seq_t((seq_t)3, n);

    /* terms of the numerator                                     */
    seq_t three_n_minus_1 = (seq_t)3 * n_seq - (seq_t)1;
    seq_t term1 = (seq_t)4 * (three_n_minus_1 * pow3);        /* 4*(3n-1)*3^n    */
    seq_t n_minus_1 = n_seq - (seq_t)1;
    seq_t term2 = (seq_t)9 * (n_minus_1 * pow2);            /* 9*(n-1)*2^n      */

    seq_t diff = term1 - term2;                    /* 4*(3n-1)*3^n - 9*(n-1)*2^n */
    seq_t num  = n_seq * diff;                     /* n * diff           */
    seq_t ans  = num / (seq_t)24;                  /* divide by 24       */

    return ans;
}
