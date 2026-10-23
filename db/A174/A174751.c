/*
 * A174751  x-values in the solution to x^2-39*y^2=1.
 * https://oeis.org/A174751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*a(n-1) - a(n-2), a(1)=1, a(2)=25
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;      /* memoization cache  */
static size_t cache_size = 0;    /* allocated size of cache   */
static idx_t   max_computed = 0; /* highest index for which cache[i] is set */

/* enlarge the cache to hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_size) return;

    size_t new_size = cache_size ? cache_size * 2 : 64;
    while (new_size < need) new_size <<= 1;        /* grow power of two */

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);                  /* allocation failed   */

    /* zero any newly allocated elements */
    for (size_t i = cache_size; i < new_size; ++i)
        tmp[i] = (seq_t)0;

    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined before offset */

    /* a(1) is known directly */
    if (n == 1) return (seq_t)1;

    ensure_cache(n);

    /* initialise first two terms if not done yet */
    if (max_computed < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)25;
        max_computed = 2;
    }

    /* compute any missing values up to n */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)50 * cache[i - 1] - cache[i - 2];
        }
        max_computed = n;
    }

    return cache[n];
}
