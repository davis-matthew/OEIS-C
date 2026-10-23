/*
 * A106707  a(n) = -A001353(n).
 * https://oeis.org/A106707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0)=0, a(1)=-1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 34, i128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized values of a(n) */
static idx_t  cache_max = 1;     /* largest index currently computed */

/* Computes a(n) for the OEIS sequence A106707. */
seq_t a(idx_t n)
{
    /* Initial allocation for n = 0 and n = 1 */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed, unlikely to happen */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-1;
        cache_max = 1;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* Resize cache array if necessary */
    if (cache_max + 1 <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return cache[cache_max]; /* allocation failed, unlikely */
        cache = new_cache;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)(4 * cache[i-1] - cache[i-2]);
    }

    cache_max = n;
    return cache[n];
}
