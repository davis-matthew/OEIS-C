/*
 * A126363  Number of base 9 n-digit numbers with adjacent digits differing by
 *          one or less.
 * https://oeis.org/A126363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑9 n‑digit strings with adjacent digits
 *            differing by ≤1. Recurrence: let dp[d] be the number of strings
 *            of current length ending in digit d (0≤d≤8). Then dp_next[d] =
 *            dp[d] + dp[d-1] (if any) + dp[d+1] (if any). a(n) = Σ dp[d] with
 *            a(0)=1. Complexity O(n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for previously computed values */
    static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) */
    static size_t cache_cap = 0;    /* size of allocated array */
    static seq_t dp_prev[9] = {0};  /* counts for current length */
    static idx_t cur_len = 0;       /* length for which dp_prev is valid */
    static int initialized = 0;     /* 0 = no data yet, 1 = ready for n≥1 */

    /* handle offset n==0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* allocate or grow cache if needed */
    if (cache_cap < (size_t)(n + 1)) {
        /* allocate new array (at least n+1 entries) */
        size_t new_cap = n + 1;
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        /* copy old data if present */
        if (cache_cap > 0 && a_cache != NULL) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_buf[i] = a_cache[i];
        }
        /* free old buffer */
        free(a_cache);
        a_cache = new_buf;
        cache_cap = new_cap;
    }

    /* if we already computed up to n, return cached value */
    if (cache_cap > 0 && a_cache[n] != 0) {
        return a_cache[n];
    }

    /* first time: initialize dp for length 1 */
    if (!initialized) {
        /* dp_prev holds counts for length 1 */
        for (int d = 0; d < 9; ++d)
            dp_prev[d] = 1;            /* single digit strings */
        /* a(1) is sum of dp_prev */
        seq_t sum = 0;
        for (int d = 0; d < 9; ++d)
            sum += dp_prev[d];
        a_cache[1] = sum;
        cur_len = 1;
        initialized = 1;
    }

    /* compute values for lengths > cur_len up to n */
    seq_t dp_cur[9];
    for (idx_t len = cur_len + 1; len <= n; ++len) {
        for (int d = 0; d < 9; ++d) {
            seq_t val = dp_prev[d];
            if (d > 0)  val += dp_prev[d - 1];
            if (d < 8)  val += dp_prev[d + 1];
            dp_cur[d] = val;
        }
        /* sum to get a(len) */
        seq_t sum = 0;
        for (int d = 0; d < 9; ++d)
            sum += dp_cur[d];
        a_cache[len] = sum;
        /* prepare for next iteration */
        for (int d = 0; d < 9; ++d)
            dp_prev[d] = dp_cur[d];
    }
    cur_len = n;

    return a_cache[n];
}
