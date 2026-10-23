/*
 * A015464  q-Fibonacci numbers for q=7, scaling a(n-2).
 * https://oeis.org/A015464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 7^(n-2) * a(n-2), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#define MAX_CACHE 20000   /* safe limit for pre‑cached values */

/* Compute the q‑Fibonacci numbers for q = 7 with scaling a(n-2) */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;     /* next free index to compute (0‑based) */
    static seq_t power7 = (seq_t)1;  /* 7^(cache_len-2) , valid when cache_len>=3 */

    /* initialise base cases on first invocation */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_len = 3;            /* indices 0,1,2 are valid */
        power7   = (seq_t)1;      /* corresponds to 7^(2-2) */
    }

    /* If requested index is already cached, return it */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* If the index exceeds our pre‑allocated cache limit,
       compute it on the fly without caching the tail. */
    if ((size_t)n >= MAX_CACHE) {
        seq_t *tmp = (seq_t*)malloc(((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        tmp[0] = (seq_t)0;
        tmp[1] = (seq_t)1;
        tmp[2] = (seq_t)1;
        seq_t tmp_pow7 = (seq_t)1;
        for (idx_t k = 3; k <= n; ++k) {
            tmp_pow7 = tmp_pow7 * (seq_t)7;                /* 7^(k-2) */
            tmp[k]   = tmp[k-1] + tmp_pow7 * tmp[k-2];
        }
        seq_t result = tmp[n];
        free(tmp);
        return result;
    }

    /* Compute missing terms up to 'n' using the recurrence */
    for (idx_t k = cache_len; k <= n; ++k) {
        power7 = power7 * (seq_t)7;                     /* 7^(k-2) */
        cache[(size_t)k] = cache[(size_t)(k-1)] + power7 * cache[(size_t)(k-2)];
    }
    cache_len = n + 1;   /* update cache length to include n */

    return cache[(size_t)n];
}
