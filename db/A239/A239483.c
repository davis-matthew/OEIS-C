/*
 * A239483  Number of (3,0)-separable partitions of n; see Comments.
 * https://oeis.org/A239483
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=1}^{⌊(n-1)/4⌋} (U(n-3m, m+1) - U(n-3m-3, m)) where
 *            U(a,b) is the number of partitions of a into exactly b parts. U
 *            satisfies U(0,0)=1, U(a,0)=0 for a>0, U(0,b)=0 for b>0 and
 *            U(a,b) = U(a-1,b-1) + U(a-b,b)
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Global cache for the unrestricted partition numbers U(n,k) */
static seq_t *dp = NULL;               /* 1‑D array storing dp[k][n] */
static size_t max_n_alloc = 0;         /* maximum n allocated */
static size_t max_k_alloc = 0;         /* maximum k allocated */

/* Allocate and (re)compute the table dp for all 0 ≤ n ≤ N and
   0 ≤ k ≤ K.  K is taken as (N/4)+2, which covers all k needed
   for a(n).  */
static void ensure_dp(idx_t N)
{
    size_t new_n = (size_t)N;
    size_t new_k = new_n / 4 + 2;      /* sufficient for all k = m+1   */
    if (new_n <= max_n_alloc && new_k <= max_k_alloc)
        return;                        /* already big enough */

    /* free old table if any */
    if (dp != NULL) {
        free(dp);
        dp = NULL;
    }
    max_n_alloc = new_n;
    max_k_alloc = new_k;
    size_t total = (max_k_alloc + 1) * (max_n_alloc + 1);
    dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (dp == NULL) {
        /* Out of memory – in such an unlikely case simply return
           zero for all answers (no sequence calculation possible). */
        return;
    }
    /* Zero initialisation of the whole table */
    memset(dp, 0, total * sizeof(seq_t));

    /* base case U(0,0) = 1 */
    dp[0] = (seq_t)1;

    /* recurrence: U(n,k) = U(n-1,k-1) + U(n-k,k) */
    for (size_t k = 1; k <= max_k_alloc; ++k) {
        for (size_t n = 1; n <= max_n_alloc; ++n) {
            seq_t val = dp[(k - 1) * (max_n_alloc + 1) + (n - 1)];
            if (n >= k) {
                val += dp[k * (max_n_alloc + 1) + (n - k)];
            }
            dp[k * (max_n_alloc + 1) + n] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 4u)
        return 0;                       /* the sequence is defined for n ≥ 4 */

    ensure_dp(n);
    if (dp == NULL)                      /* allocation failed */
        return 0;

    seq_t ans = 0;
    idx_t max_m = (n - 1u) / 4u;          /* largest possible number of 3’s */

    for (idx_t m = 1u; m <= max_m; ++m) {
        idx_t S = n - 3u * m;            /* sum of the non‑3 parts   */
        idx_t k = m + 1u;                /* their number            */

        seq_t term1 = dp[k * (max_n_alloc + 1) + S];

        seq_t term2 = 0;
        if (S >= 3u) {
            idx_t S2 = S - 3u;           /* remove one more 3       */
            /* k-1 ≤ max_k_alloc automatically holds */
            term2 = dp[(k - 1) * (max_n_alloc + 1) + S2];
        }

        ans += term1;
        ans -= term2;
    }
    return ans;
}
