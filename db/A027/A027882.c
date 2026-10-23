/*
 * A027882  a(n) = Sum_{k>=1} k^n (2/3)^k.
 * https://oeis.org/A027882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} k^n (2/3)^k (offset 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes OEIS sequence A027882.  Positive-indexing for n >= 0. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;   /* cache[n] = a(n)                  */
    static size_t cache_cap  = 0;       /* capacity of cache array          */
    static seq_t *dp         = NULL;   /* dp[k] = b(current_n, k)   */
    static size_t dp_cap     = 0;       /* capacity of dp array             */
    static idx_t  max_n       = 0;      /* highest n for which we have data */

    /*--- initialise cache for n = 0 ---*/
    if (cache_cap == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;           /* a(0) = 2 */
        cache_cap = 1;
        max_n = 0;
    }

    /* if the value is already known, return it */
    if (n <= max_n) return cache[n];

    /*--- ensure the temporary array is large enough ---*/
    size_t need = n + 1;                  /* we need indices 0..n for dp */
    if (dp_cap < need) {
        seq_t *tmp = (seq_t*)realloc(dp, need * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise the newly allocated part to zero */
        for (size_t i = dp_cap; i < need; ++i) tmp[i] = 0;
        dp = tmp;
        dp_cap = need;
    }

    /*--- compute all a(k) for k = max_n + 1 .. n ---*/
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        if (cur == 1) {
            /* b(1,1) = 2  (see recurrence below) */
            dp[0] = 0;          /* dp[0] is unused; keep it zero */
            dp[1] = (seq_t)2;
        } else {
            /* recurrence: b(n,k) = k*b(n-1,k) + 2*k*b(n-1,k-1) */
            for (idx_t k = cur; k >= 1; --k) {
                seq_t old_k   = dp[k];     /* b(n-1,k)  */
                seq_t old_km1 = dp[k-1];   /* b(n-1,k-1)*/
                dp[k] = (seq_t)k * old_k + (seq_t)(2*k) * old_km1;
            }
        }

        /* a(cur) = 3 * sum_{k=1..cur} b(cur,k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) sum += dp[k];
        cache[cur] = (seq_t)3 * sum;
    }

    max_n = n;
    return cache[n];
}
