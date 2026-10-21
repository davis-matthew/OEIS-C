/*
 * A073357  Binomial transform of tribonacci numbers.
 * https://oeis.org/A073357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3), a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A073357 (Binomial transform of tribonacci numbers). */
seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary call order and fast repeated look‑ups. */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t   cache_len = 0;     /* number of terms stored (most recent index + 1) */
    static size_t   cache_cap = 0;     /* allocated capacity */

    /* Helper: grow the cache to at least 'need' capacity. */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms if not yet done. */
    if (cache_len < 3) {
        /* Ensure we have space for indices 0,1,2 */
        if (cache_cap < 3) {
            size_t new_cap = 3;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        if (cache_len < 3) cache_len = 3;
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_len) return cache[n];

    /* Compute successive terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term;
        if (i == 0) { term = 0; }
        else if (i == 1) { term = 1; }
        else if (i == 2) { term = 3; }
        else {
            /* a(i) = 4*a(i-1) - 4*a(i-2) + 2*a(i-3) */
            term = cache[i-1] * 4;
            term -= cache[i-2] * 4;
            term += cache[i-3] * 2;
        }
        cache[i] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
