/*
 * A070047  Number of partitions of n in which no part appears more than twice
 *          and no two parts differ by 1.
 * https://oeis.org/A070047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n in which no part appears more than
 *            twice and no two parts differ by 1. This is equivalent to
 *            counting assignments of multiplicities m_i ∈ {0,1,2} to each
 *            integer i, with the restriction that if m_i > 0 then m_{i-1} =
 *            m_{i+1} = 0. The DP below computes the number of ways...
 * Range:     u8 n <= 27, u16 n <= 76, u32 n <= 235, u64 n <= 787,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined by the build. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Zero or tiny n are trivial. */
    if (n == 0) return (seq_t)1;
    size_t N = (size_t)n + 1;

    /* Allocate four arrays of size N:
       dp_no   – ways to reach a sum with the previous part unused
       dp_used – ways to reach a sum with the previous part used
       next_no, next_used – buffers for the next iteration   */
    seq_t *dp_no   = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *dp_used = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *next_no = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *next_used = (seq_t *)calloc(N, sizeof(seq_t));
    if (!dp_no || !dp_used || !next_no || !next_used) {
        /* Memory allocation failed – return 0 as a safe sentinel. */
        free(dp_no); free(dp_used); free(next_no); free(next_used);
        return (seq_t)0;
    }

    /* Base case: empty partition of sum 0. */
    dp_no[0] = (seq_t)1;

    /* Process parts 1..n in increasing order. */
    for (idx_t part = 1; part <= n; ++part)
    {
        /* Compute next_no[s] = dp_no[s] + dp_used[s]  (skip this part). */
        for (size_t s = 0; s < N; ++s) {
            next_no[s] = dp_no[s] + dp_used[s];
        }

        /* Compute next_used[s] – use this part once or twice. */
        for (size_t s = 0; s < N; ++s) {
            next_used[s] = (seq_t)0;
        }
        size_t p = (size_t)part;
        for (size_t s = p; s < N; ++s) {
            next_used[s] = dp_no[s - p];                  /* use once */
            if (s >= 2 * p) {
                next_used[s] = next_used[s] + dp_no[s - 2 * p]; /* use twice */
            }
        }

        /* Swap current and next arrays for the next iteration. */
        seq_t *tmp = dp_no;   dp_no   = next_no;   next_no   = tmp;
        tmp = dp_used;        dp_used = next_used; next_used = tmp;
    }

    /* The answer is the sum of both states for sum n. */
    seq_t result = dp_no[n] + dp_used[n];

    /* Clean up. */
    free(dp_no); free(dp_used); free(next_no); free(next_used);

    return result;
}
