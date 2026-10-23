/*
 * A170861  a(0) = 1, a(1) = 0; a(2^i + j) = 2a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0; a(2^i + j)=2*a(j)+3*a(j+1) for 0 <= j < 2^i
 * Range:     u8 n <= 27, u16 n <= 503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity */
    static size_t len   = 0;     /* next index to compute (cache[0..len-1] valid) */

    /* make sure the cache is large enough */
    if (cap < n + 1) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                  /* allocation failed – use new if needed   */
            tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* fallback – unlikely in test          */
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the base values once */
    if (len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        len     = 2;
    }

    /* if the result is already in the cache, just return it */
    if (n < len) return cache[n];

    /* compute terms up to n inclusively */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* i >= 2 by construction of the loop */
        idx_t hi = (idx_t)1 << (63 - __builtin_clzll(i));   /* highest power of two <= i */
        idx_t j  = i - hi;                                 /* 0 <= j < hi */
        seq_t v1 = cache[j];
        seq_t v2 = cache[j + 1];
        cache[i] = (seq_t)( (seq_t)2 * v1 + (seq_t)3 * v2 );
    }

    len = (size_t)(n + 1);   /* update cached length */
    return cache[n];
}
