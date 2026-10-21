/*
 * A099451  A Chebyshev transform of A099450 associated to the knot 7_7.
 * https://oeis.org/A099451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=5, a(2)=17, a(3)=45,
 *            a(n)=5*a(n‑1)−9*a(n‑2)+5*a(n‑3)−a(n‑4) for n≥4
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 23, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the recurrence derived from the generating function
   (1+x^2)/(1−5x+9x^2−5x^3+x^4).  */

#include <stdlib.h>
#include <assert.h>

/* Cached sequence values.  */
static seq_t *cache = NULL;          /* Dynamic array of seq_t */
static idx_t cache_size = 0;         /* Number of elements allocated */

/* Extend cache to at least size n+1 and compute missing terms. */
static void ensure_cache(idx_t n)
{
    /* If already computed, nothing to do. */
    if (n + 1 <= cache_size)
        return;

    /* Reallocate cache to hold n+1 elements. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();  /* Allocation failed */

    cache = new_cache;
    idx_t old_size = cache_size;
    cache_size = new_size;

    /* Initialise the first four terms, if the cache is large enough. */
    if (cache_size >= (idx_t)1) cache[0] = (seq_t)1;
    if (cache_size >= (idx_t)2) cache[1] = (seq_t)5;
    if (cache_size >= (idx_t)3) cache[2] = (seq_t)17;
    if (cache_size >= (idx_t)4) cache[3] = (seq_t)45;

    /* Compute new terms using the recurrence.  */
    idx_t i_start = old_size;
    if (i_start < (idx_t)4)
        i_start = (idx_t)4;           /* Recurrence needs 4 past terms */
    for (idx_t i = i_start; i < cache_size; ++i)
    {
        seq_t term = 5 * cache[i - 1]
                    - 9 * cache[i - 2]
                    + 5 * cache[i - 3]
                    -  cache[i - 4];
        cache[i] = term;
    }
}

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
