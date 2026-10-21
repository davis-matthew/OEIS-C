/*
 * A215005  a(n) = a(n-2) + a(n-1) + floor(n/2) + 1 for n > 1 and a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A215005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(0)=0, a(1)=1, a(n)=a(n-2)+a(n-1)+floor(n/2)+1 for n>1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t computed_max = 0;/* highest index computed so far */

    /* initialise cache with the two seed values */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed_max = 1;
    }

    /* trivial cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow the array if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* fill missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = cache[i-2] + cache[i-1] + (seq_t)(i / 2) + (seq_t)1;
    }
    computed_max = n;

    return cache[n];
}
