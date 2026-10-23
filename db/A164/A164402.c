/*
 * A164402  Number of binary strings of length n with no substrings equal to
 *          0010 or 0110.
 * https://oeis.org/A164402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4); a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=8
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache: dynamic array to memoize results */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t  cache_size = 0; /* number of terms currently stored (>=0) */
    static size_t cache_cap   = 0; /* current capacity of the array */

    /* Initial terms (for n < 4) */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)4;
            case 3: return (seq_t)8;
            default: break; /* unreachable */
        }
    }

    /* Make sure the cache has room up to n */
    if (cache == NULL) {
        /* First allocation: start with a modest size */
        cache_cap   = 64;
        cache_size  = 0;
        cache       = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
    }
    if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Pre-fill cache with the initial four terms if not already done */
    if (cache_size < 4) {
        if (cache_size <= 0) cache[0] = (seq_t)1;
        if (cache_size <= 1) cache[1] = (seq_t)2;
        if (cache_size <= 2) cache[2] = (seq_t)4;
        if (cache_size <= 3) cache[3] = (seq_t)8;
        cache_size = 4;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 2*a(i-3) + 2*a(i-4) */
        seq_t term = 2 * cache[i - 1];
        term -= 2 * cache[i - 3];
        term += 2 * cache[i - 4];
        cache[i] = term;
    }
    cache_size = n + 1;

    return cache[n];
}
