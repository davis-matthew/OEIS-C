/*
 * A190983  a(n) = 9*a(n-1) - 6*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 6*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Computes the n-th term of sequence A190983.
 * The function memoizes results in a static array, allowing
 * arbitrary order of calls without recomputation. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;  /* size of allocated array */
    static idx_t  cache_max  = 0; /* highest index computed */

    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* if cache uninitialized or too small, allocate/resize */
    if (cache == NULL || n >= cache_cap) {
        idx_t new_cap = n + 1;                    /* need indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {                  /* allocation failed */
            abort();                              /* cannot recover */
        }
        /* preserve previous values */
        if (cache == NULL) {
            /* initialise base values */
            new_cache[0] = (seq_t)0;
            new_cache[1] = (seq_t)1;
            cache_max = 1;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = ((seq_t)9ULL * cache[i - 1]) - ((seq_t)6ULL * cache[i - 2]);
    }
    cache_max = n;

    return cache[n];
}
