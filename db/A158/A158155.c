/*
 * A158155  Number of nondecreasing integer sequences of length 21 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0}^{\min(21,n)} \sum_{l=0}^{\min(21-k,n)}
 *            P(n,k)\,P(n,l) where P(n,k) is the number of partitions of n
 *            into exactly k parts
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 46, u64 n <= 205, u128 n <= 304,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts nondecreasing integer sequences of length 21
   whose sum is 0 and whose sum of absolute values is 2n. */

#include <stdlib.h>
#include <string.h>

#define MAX_K 21  /* maximum number of parts in a partition              */

/* 1‑D index into the DP table dp[n][k]                                 */
static inline idx_t IDX(idx_t n, idx_t k)
{
    return n * (idx_t)(MAX_K + 1) + k;
}

/* DP table of partitions: dp[n][k] = P(n,k)                           */
static seq_t *dp        = NULL;  /* pointer to (max_n+1)*(MAX_K+1) array */
static idx_t max_n_comp = 0;     /* largest n for which dp is valid       */

/* Allocate / extend table up to n_max and fill missing rows         */
static void ensure_dp(idx_t n_max)
{
    if (dp == NULL || max_n_comp < n_max) {
        size_t old_size = (size_t)(max_n_comp + 1) * (MAX_K + 1);
        size_t new_size = (size_t)(n_max + 1) * (MAX_K + 1);

        seq_t *newdp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (newdp == NULL) abort();
        dp = newdp;

        /* Zero the newly added rows if any */
        if (old_size < new_size) {
            size_t add_rows = new_size - old_size;
            memset(dp + old_size, 0, add_rows * sizeof(seq_t));
        }

        /* Base case: P(0,0)=1, all other P(0,k>0)=0 (already zeroed) */
        dp[IDX(0, 0)] = (seq_t)1;

        /* Compute DP rows from max_n_comp+1 up to n_max                 */
        for (idx_t n = max_n_comp + 1; n <= n_max; ++n) {
            dp[IDX(n, 0)] = (seq_t)0; /* no positive parts sum to n>0      */
            for (idx_t k = 1; k <= MAX_K; ++k) {
                if (k > n) {
                    dp[IDX(n, k)] = (seq_t)0;
                } else {
                    seq_t val = dp[IDX(n - 1, k - 1)]; /* one part is 1   */
                    if (n >= k)                      /* all parts ≥ 2   */
                        val += dp[IDX(n - k, k)];
                    dp[IDX(n, k)] = val;
                }
            }
        }
        max_n_comp = n_max;
    }
}

/* Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1; 0 is unused but safe */

    ensure_dp(n);                    /* make sure partitions up to n    */

    idx_t kmax = (n < MAX_K) ? n : MAX_K;
    seq_t sum  = (seq_t)0;

    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t pk = dp[IDX(n, k)];
        if (pk == 0) continue;

        idx_t lmax = (k <= MAX_K) ? ((k <= n) ? n : MAX_K) : 0; /* l ≤ n and l ≤ 21‑k */
        if (lmax > (idx_t)MAX_K - k) lmax = (idx_t)MAX_K - k;
        if (lmax > n) lmax = n;

        for (idx_t l = 0; l <= lmax; ++l) {
            seq_t pl = dp[IDX(n, l)];
            if (pl == 0) continue;
            sum += pk * pl;
        }
    }
    return sum;
}
