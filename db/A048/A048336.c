/*
 * A048336  a(n) in base 12 is a repdigit.
 * https://oeis.org/A048336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 46, u32 n <= 97, u64 n <= 194,
 *            u128 n <= 390, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 13*a(n-11) - 12*a(n-22)  for n > 21
   Initial terms: a(0)=0, a(1)=1, …, a(21)=130. */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* first 22 values (indices 0 … 21) */
    static const seq_t init[22] = {
        (seq_t)0,(seq_t)1,(seq_t)2,(seq_t)3,(seq_t)4,(seq_t)5,
        (seq_t)6,(seq_t)7,(seq_t)8,(seq_t)9,(seq_t)10,(seq_t)11,
        (seq_t)13,(seq_t)26,(seq_t)39,(seq_t)52,(seq_t)65,
        (seq_t)78,(seq_t)91,(seq_t)104,(seq_t)117,(seq_t)130
    };

    /* allocate initial storage if needed */
    if (cache == NULL) {
        cache_cap = 32;                 /* start with a small capacity */
        cache    = seq_alloc(cache_cap); /* allocates an array of cache_cap seq_t */
        cache_len = 0;
    }

    /* extend the cache until it can hold index n */
    while (n + 1 > cache_cap) {
        idx_t old_cap = cache_cap;
        cache_cap <<= 1;                /* double the capacity */
        cache = seq_realloc(cache, old_cap, cache_cap);
    }

    /* compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 22)
            cache[i] = init[i];
        else
            cache[i] = (seq_t)13 * cache[i - 11] - (seq_t)12 * cache[i - 22];
    }
    cache_len = n + 1;      /* all entries 0 … n are now computed */

    return cache[n];
}
