/*
 * A152587  Generalized Fermat numbers: a(n) = 14^(2^n) + 1.
 * https://oeis.org/A152587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)-1)^2 + 1, a(0) = 15
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises values in a growable array. */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache shared across calls */
static seq_t *cache = NULL;     /* pointer to array of values */
static size_t cache_cap = 0;    /* allocated size of the array (in elements) */
static size_t cache_len = 0;    /* number of elements currently stored */

/* Ensure that the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap >= need)
        return;   /* already enough space */

#ifdef __cplusplus
    /* Allocate a new array of the required size using C++ new[] */
    seq_t *tmp = new seq_t[need];
    /* Copy old values to the new array */
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    /* Delete the old array */
    delete [] cache;
    cache = tmp;
#else
    /* Allocate a new array of the required size using realloc */
    seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
    if (tmp == NULL) abort();  /* memory allocation failed */
    cache = tmp;
#endif
    cache_cap = need;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    /* Compute missing terms starting from cache_len */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)15;                     /* a(0) = 15 */
        } else {
            seq_t prev = cache[i-1];
            seq_t tmp  = prev - (seq_t)1;             /* a(i-1)-1   */
            cache[i] = tmp * tmp + (seq_t)1;          /* (a(i-1)-1)^2+1 */
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
