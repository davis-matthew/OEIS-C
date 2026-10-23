/*
 * A108581  Positive triangular numbers repeated their own number of times.
 * https://oeis.org/A108581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1930, u16 n <= 9551, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implementation of OEIS sequence A108581: 
 * Positive triangular numbers repeated their own number of times.
 * For n ≥ 1 the value a(n) is A000217(k) where k is the unique integer
 * satisfying
 *      Σ_{i=1}^{k-1} T(i) < n ≤ Σ_{i=1}^{k} T(i),
 * with T(i)=i(i+1)/2.
 * The algorithm simply accumulates triangular numbers until the
 * cumulative sum reaches or exceeds n.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cum = 0;
    seq_t m   = 1;

    for (;;)
    {
        /* current triangular number */
        seq_t tri = m * (m + 1) / 2;

        /* add to cumulative sum */
        cum += tri;

        /* if we have covered position n, return this triangular number */
        if (cum >= (seq_t)n)
            return tri;

        /* move to next m */
        m = m + 1;
    }
}
