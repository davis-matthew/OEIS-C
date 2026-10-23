/*
 * A126490  Number of base 22 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence: A126490 – Number of base 22 n‑digit numbers with adjacent digits
   differing by at most 3.
   Computation:  a(0) = 1.  For n ≥ 1 let dp[d] be the number of length‑n
   sequences ending with digit d (0 ≤ d < 22).  Transition:
        dp_next[d] = Σ_{|d′‑d| ≤ 3} dp[d′].
   The value a(n) is Σ_{d} dp[d].
   This implementation memoises results and uses O(1) extra memory per
   call, running in O(n·22·7). */

seq_t a(idx_t n)
{
    /* Return the empty string case immediately. */
    if (n == 0)
        return (seq_t)1;

    /* Static cache for all computed values a(0)…a(cur_n). */
    static seq_t *cache      = NULL;   /* cache[i] = a(i)          */
    static size_t cache_cap  = 0;      /* allocated size of cache  */
    static size_t cache_size = 0;      /* number of valid entries  */

    /* Static dynamic‑programming arrays. */
    static seq_t *dp_prev   = NULL;    /* counts for length cur_n   */
    static seq_t *dp_curr   = NULL;    /* temporary counts          */
    static idx_t  cur_n     = 0;       /* largest n computed       */

    /* Initialise cache if this is the first call. */
    if (cache_size == 0) {
        cache_cap  = 8;                     /* start with small capacity */
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0]   = (seq_t)1;              /* a(0) = 1                  */
        cache_size = 1;
    }

    /* Ensure the cache can hold up to n. */
    if (cache_size <= (size_t)n) {
        while (cache_cap <= (size_t)n)
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* If the requested value is already cached, return it. */
    if (cache_size > (size_t)n)
        return cache[n];

    /* First time we need to compute lengths > 0.  Allocate DP arrays
       and initialise the counts for length 1 (each digit appears once). */
    if (cur_n == 0) {
        dp_prev = (seq_t *)malloc(sizeof(seq_t) * 22);
        dp_curr = (seq_t *)malloc(sizeof(seq_t) * 22);
        for (int d = 0; d < 22; ++d)
            dp_prev[d] = (seq_t)1;          /* length‑1 strings */
        cache[1] = (seq_t)22;              /* a(1) = 22           */
        cur_n   = 1;
        cache_size = 2;                    /* we have a(0) and a(1) */
    }

    /* Incrementally compute values up to n. */
    while (cur_n < n) {
        /* Zero out the current array. */
        for (int i = 0; i < 22; ++i)
            dp_curr[i] = (seq_t)0;

        /* Transition: for each ending digit d, distribute its count to
           all digits d' with |d'‑d| ≤ 3. */
        for (int d = 0; d < 22; ++d) {
            seq_t val = dp_prev[d];
            if (val == (seq_t)0) continue;
            for (int off = -3; off <= 3; ++off) {
                int d2 = d + off;
                if (d2 >= 0 && d2 < 22)
                    dp_curr[d2] += val;
            }
        }

        /* Sum all dp_curr entries to obtain a(cur_n+1). */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 22; ++i)
            total += dp_curr[i];

        ++cur_n;
        cache[cur_n] = total;
        if (cache_size <= cur_n)
            cache_size = cur_n + 1;

        /* Prepare for next iteration: swap dp_prev and dp_curr. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    return cache[n];
}
