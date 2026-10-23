/*
 * A047520  a(n) = 2*a(n-1) + n^2, a(0) = 0.
 * https://oeis.org/A047520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + n^2, a(0) = 0 (OEIS A047520)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of pre‑computed terms (next free index) */
    static idx_t cache_cap  = 0;    /* allocated capacity (in seq_t elements) */

    /* grow cache if needed */
    if ((size_t)n >= (size_t)cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {            /* allocation failure – fall back to malloc */
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp)                          /* very unlikely for n <= 10000 */
                return (seq_t)0;               /* undefined behaviour suppressed */
        }
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialize first term if cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        cache[i] = cache[i - 1] * (seq_t)2 + i_seq * i_seq;
    }

    cache_size = n + 1;
    return cache[n];
}
