/*
 * A168689  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 12 a(n) = 10 * (a(n-1)+a(n-2)+…+a(n-16)) – 55 *
 *            a(n-17) , n ≥ 18 (The first 18 terms are listed below.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 18 terms of the sequence */
    static const seq_t init[18] = {
        (seq_t)1,
        (seq_t)12,
        (seq_t)132,
        (seq_t)1452,
        (seq_t)15972,
        (seq_t)175692,
        (seq_t)1932612,
        (seq_t)21258732,
        (seq_t)233846052,
        (seq_t)2572306572,
        (seq_t)28295372292,
        (seq_t)311249095212,
        (seq_t)3423740047332,
        (seq_t)37661140520652,
        (seq_t)414272545727172,
        (seq_t)4556998002998892,
        (seq_t)50126978032987812,
        (seq_t)551396758362865866
    };

    /* For indices 0 … 17 we return the known values directly. */
    if (n <= 17) {
        return init[n];
    }

    /* Allocate an array to hold the computed values up to n. */
    seq_t *a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (a == NULL) {
        /* Allocation failure – return 0 as a sentinel. */
        return (seq_t)0;
    }

    /* Copy the known initial values. */
    for (idx_t i = 0; i <= 17; ++i) {
        a[i] = init[i];
    }

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 18; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 16; ++j) {
            sum += a[i - j];
        }
        sum = sum * (seq_t)10;                     /* 10 * sum_{k=1}^{16} a(i-k) */
        sum = sum - ((seq_t)55 * a[i - 17]);       /* – 55 * a(i-17)          */
        a[i] = sum;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
