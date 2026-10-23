/*
 * A180911  Number of sequences of n integers p(i) i=0..n-1 with 0<=p(i)<=6*i
 *          and -6<p(i)-p(i-1)<=6.
 * https://oeis.org/A180911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A180911
 * a(n) = number of sequences p(i), i=0..n-1, with
 *   0 ≤ p(i) ≤ 6*i  and  -6 < p(i)-p(i-1) ≤ 6   (p(-1) is undefined)
 *
 * The implementation uses dynamic programming: for each i the vector
 * dp[v] counts the sequences of length i+1 ending at p(i)=v.
 * Transition:
 *   dp_next[v] = Σ_{d=-5}^{6} dp[v-d]   (indices clamped to valid ranges)
 * The total a(n) is the sum of dp[v] for i=n-1.
 *
 * A memoisation cache stores all previously computed a(k), k ≤ n,
 * so subsequent calls are O(n-i) where i is the largest previously
 * requested n.  The algorithm runs in O(n²) time and O(n) memory
 * per call, easily fast enough for n up to 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoisation */
static seq_t *dp_cur    = NULL;      /* DP vector for current row */
static idx_t  dp_size   = 0;         /* size of dp_cur  (6*i+1)   */
static idx_t  dp_i      = 0;         /* row index i of dp_cur (0‑based) */

/* Cache of a(n) values: cache[k] == a(k) for 1 ≤ k ≤ cache_sz */
static seq_t *cache     = NULL;
static idx_t  cache_sz   = 0;

/* Ensure cache has if (n) entries (indices 0..n).  Zero‑initialise
 * newly added entries.                                                    */
static void ensure_cache(idx_t n)
{
    if (n <= cache_sz) return;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();        /* out of memory */
    for (idx_t i = cache_sz + 1; i <= n; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_sz = n;
}

/* Compute a(n) for any n ≥ 1.  The function is external linkage.     */
seq_t a(idx_t n)
{
    /* Return cached value if already known. */
    if (n <= cache_sz) return cache[n];
    /* Initialise on first call.  dp[0] corresponds to i = 0. */
    if (dp_cur == NULL) {
        dp_cur = (seq_t *)malloc(sizeof(seq_t));  /* size 1   */
        dp_cur[0] = (seq_t)1;                     /* only p(0)=0 */
        dp_size = 1;
        dp_i = 0;
        ensure_cache(1);
        cache[1] = (seq_t)1;                     /* a(1) = 1  */
    }

    /* Extend the DP rows up to i = n-1 (length n). */
    for (idx_t i = dp_i + 1; i <= n - 1; ++i) {
        idx_t new_size = 6 * i + 1;              /* size of new row    */
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));

        /* Prefix sums of the previous row for O(1) range sums. */
        seq_t *pref = (seq_t *)malloc((dp_size + 1) * sizeof(seq_t));
        pref[0] = (seq_t)0;
        for (idx_t j = 0; j < dp_size; ++j) pref[j + 1] = pref[j] + dp_cur[j];

        seq_t row_sum = (seq_t)0;                 /* a(i+1) = total sum of row */

        for (idx_t v = 0; v < new_size; ++v) {
            idx_t low  = (v > 6) ? v - 6 : 0;        /* clamp left boundary   */
            idx_t high = v + 5;                     /* clamp right boundary  */
            if (high >= dp_size) high = dp_size - 1;

            /* Sum of dp_cur[low..high] via prefix sums */
            new_dp[v] = pref[high + 1] - pref[low];
            row_sum  += new_dp[v];
        }

        free(pref);
        free(dp_cur);          /* free previous row */

        dp_cur = new_dp;       /* advance to new row          */
        dp_size = new_size;
        dp_i = i;              /* now dp_cur corresponds to row i */

        /* Cache the computed a(i+1) */
        ensure_cache(i + 1);
        cache[i + 1] = row_sum;
    }

    return cache[n];
}
