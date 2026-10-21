/*
 * A200069  a(n) = 4*a(n-1) + 13*a(n-2) for n>2, a(1)=1, a(2)=4.
 * https://oeis.org/A200069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) + 13*a(n-2)   (n>2)
 * Initial values: a(1) = 1, a(2) = 4
 *
 * Computes only what is needed and memoises results so that calls in any
 * order are fast (O(1) after the first expansion).  All arithmetic is
 * performed in the provided integer type seq_t, which may be 8/16/32/64-bit
 * unsigned, unsigned __int128, or an arbitrary precision integer type.
 *
 * The function is thread-unsafe; this is acceptable in the current
 * evaluation context.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache to store computed terms.  Index 0 holds a(1). */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;     /* number of terms stored */

    if (n < 1)
        return 0;   /* undefined for n==0, but return 0 */

    /* Ensure cache contains at least the first two terms. */
    if (cache == NULL) {
        cache_sz = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_sz);
        if (cache == NULL)  /* allocation failure – return 0 */
            return 0;
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)4;  /* a(2) */
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_sz)
        return cache[n - 1];

    /* Expand the cache up to n. */
    size_t new_sz = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (tmp == NULL)  /* reallocation failure – return 0 */
        return 0;
    cache = tmp;
    cache_sz = new_sz;

    /* Compute new terms using the recurrence. */
    for (size_t i = 2; i < cache_sz; ++i) { /* i corresponds to a(i+1) */
        cache[i] = (seq_t)4 * cache[i - 1] + (seq_t)13 * cache[i - 2];
    }

    /* Return the requested term. */
    return cache[n - 1];
}
