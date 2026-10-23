/*
 * A158168  Number of nondecreasing integer sequences of length 34 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{34} sum_{m=1}^{34-k} (p_k(n) - p_{k-1}(n)) *
 *            (p_m(n) - p_{m-1}(n)) where p_k(n) is the number of partitions
 *            of n into at most k parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 143, u128 n <= 222,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>

/* Function definition */
seq_t a(idx_t n)
{
    /* Limit on the number of parts */
    idx_t K = (n < 34) ? (idx_t)n : 34ULL;

    /* Allocate arrays for DP */
    seq_t *prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!prev || !curr) {
        /* memory allocation failed – return 0 */
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Initial condition: partitions of 0 into at most 0 parts */
    prev[0] = (seq_t)1;

    /* diff[k] = p_k(n) - p_{k-1}(n) for k = 1..K */
    seq_t *diff = (seq_t *)calloc((size_t)(K + 1), sizeof(seq_t));
    if (!diff) {
        free(prev); free(curr);
        return (seq_t)0;
    }

    /* DP to compute p_k(n) for k = 1..K */
    for (idx_t k = 1; k <= K; ++k) {
        for (idx_t s = 0; s <= n; ++s) {
            curr[s] = prev[s];
            if (s >= k) {
                curr[s] += curr[s - k];
            }
        }
        diff[k] = curr[n] - prev[n];
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute sum of diff values = p(n) */
    seq_t sumDiff = (seq_t)0;
    for (idx_t k = 1; k <= K; ++k) {
        sumDiff += diff[k];
    }

    /* total = p(n)^2 */
    seq_t total = sumDiff * sumDiff;

    /* Prefix sums of diff for fast range sum */
    seq_t *pref = (seq_t *)calloc((size_t)(K + 1), sizeof(seq_t));
    if (!pref) {
        free(prev); free(curr); free(diff);
        return (seq_t)0;
    }
    pref[0] = (seq_t)0;
    for (idx_t i = 1; i <= K; ++i) {
        pref[i] = pref[i - 1] + diff[i];
    }

    /* Subtract contributions where k + m > 34 */
    seq_t sub = (seq_t)0;
    for (idx_t k = 1; k <= K; ++k) {
        idx_t m_start = 35ULL - k;         /* minimal m with k+m > 34 */
        if (m_start < 1ULL) m_start = 1ULL;
        if (m_start > K) continue;         /* no m satisfies the condition */
        seq_t partial = pref[K] - pref[m_start - 1];
        sub += diff[k] * partial;
    }

    /* Result */
    seq_t result = total - sub;

    /* Free memory */
    free(prev);
    free(curr);
    free(diff);
    free(pref);

    return result;
}
