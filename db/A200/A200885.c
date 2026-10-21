/*
 * A200885  Number of 0..7 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors.
 * https://oeis.org/A200885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A200885: number of 0..7 arrays of length n+2 with no interior element
   strictly greater than both neighbors.  Computation is by a dynamic
   programming over the last two elements; each state (p,q) counts
   sequences ending with p,q.  Transition forbids the pattern p<q>y. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n<1, return 0 */

    idx_t L = n + 2;                  /* total length of the array */

    /* dp[p][q] holds the number of sequences of current length whose
       last two elements are p followed by q.  8×8 since elements are 0..7. */
    seq_t dp[8][8] = {0};

    /* All pairs are allowed for length 2. */
    for (int p = 0; p < 8; ++p)
        for (int q = 0; q < 8; ++q)
            dp[p][q] = 1;

    /* Extend the sequence from length 3 up to L. */
    for (idx_t step = 3; step <= L; ++step) {
        seq_t next[8][8] = {0};

        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                seq_t val = dp[p][q];
                if (!val)
                    continue;          /* skip zero states */

                for (int y = 0; y < 8; ++y) {
                    if (p < q && q > y)   /* local maximum forbidden */
                        continue;
                    next[q][y] += val;
                }
            }
        }

        /* copy next into dp for the next iteration */
        for (int p = 0; p < 8; ++p)
            for (int q = 0; q < 8; ++q)
                dp[p][q] = next[p][q];
    }

    /* Sum all states: total number of valid arrays. */
    seq_t total = 0;
    for (int p = 0; p < 8; ++p)
        for (int q = 0; q < 8; ++q)
            total += dp[p][q];

    return total;
}
