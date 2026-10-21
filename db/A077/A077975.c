/*
 * A077975  Expansion of 1/(1+x+x^2-2*x^3).
 * https://oeis.org/A077975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-2) - a(n-1) with initial values a(0)=1,
 *            a(1)=-1, a(2)=0
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 48, i64 n <= 97, i128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;      /* dynamic array of terms */
static size_t cache_len = 0;     /* number of terms currently stored */

/* Make sure the cache contains all terms up to index n */
static void ensure_cache(idx_t n)
{
    /* If we already have enough terms, nothing to do */
    if (cache_len > (size_t)n)
        return;

    /* Allocate space for indices 0..n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)      /* allocation failed – keep previous state, caller will crash */
        return;
    cache = tmp;

    /* If this is the first allocation, initialise the first three terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)0;
        cache_len = 3;
    }

    /* Compute any new terms that have not yet been calculated */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i - 3] - cache[i - 2] - cache[i - 1];
        cache[i] = val;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* The three initial terms are known directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)0;

    /* Ensure the cache contains the desired term */
    ensure_cache(n);
    return cache[(size_t)n];
}
