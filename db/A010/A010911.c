/*
 * A010911  Pisot sequence E(3,11), a(n) = floor(a(n-1)^2/a(n-2) + 1/2).
 * https://oeis.org/A010911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3) for n ≥ 3 a(0) = 3, a(1) =
 *            11, a(2) = 40
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms      */
static idx_t  cache_size = 0;        /* current allocation size     */
static idx_t  max_computed = (idx_t)-1; /* index of largest computed term */

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = (n < 16) ? 16 : n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        /* zero newly allocated entries */
        for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* Initial values are defined for n = 0, 1, 2 */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)40;

    /* On first call, create cache and store base cases */
    if (cache == NULL) {
        ensure_cache(n);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)40;
        max_computed = 2;
    }

    /* Extend the cache if necessary */
    if (cache_size <= n) {
        ensure_cache(n);
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) + 2*a(i-2) + a(i-3) */
        seq_t term =   (seq_t)3 * cache[i - 1]
                      + (seq_t)2 * cache[i - 2]
                      +        cache[i - 3];
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
