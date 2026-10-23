/*
 * A017878  Expansion of 1/(1-x^9-x^10-x^11).
 * https://oeis.org/A017878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) + a(n-10) + a(n-11) for n≥12; a(0)=1;
 *            a(1)..a(8)=0; a(9)=a(10)=a(11)=1
 * Range:     u8 n <= 67, u16 n <= 118, u32 n <= 222, u64 n <= 423,
 *            u128 n <= 825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc */

/* Compute the n-th term of OEIS A017878 */
seq_t
a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;    /* allocated capacity */
    static idx_t  max_computed = 0;  /* largest index already in cache */

    /* Allocate or grow the cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        /* grow to at least 64, then double until large enough */
        cache_cap = old_cap > 0 ? old_cap << 1 : 64;
        while (cache_cap <= (size_t)n)
            cache_cap <<= 1;
        /* realloc the buffer */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: return 0 (undefined) */
        cache = tmp;
        /* initialize newly added slots to zero */
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* Ensure the base value a(0)=1 is present */
    if (max_computed == 0 && cache[0] == 0)
        cache[0] = 1;

    /* Compute any missing values up to n */
    for (size_t i = (size_t)max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 8) {
            /* a(1)..a(8) are already zero */
            continue;
        } else if (i <= 11) {
            cache[i] = 1;
        } else {
            /* a(i) = a(i-9) + a(i-10) + a(i-11) */
            cache[i] = cache[i - 9] + cache[i - 10] + cache[i - 11];
        }
    }
    max_computed = n;

    return cache[n];
}
