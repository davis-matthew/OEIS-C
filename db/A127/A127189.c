/*
 * A127189  E.g.f.: sqrt((1+4*x)/(1+2*x)).
 * https://oeis.org/A127189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) + (6*n-7)*a(n-1) + 8*(n-1)*(n-2)*a(n-2) = 0
   Initial values: a(0)=1, a(1)=1 */
#include <stddef.h>
#include <stdlib.h>

/* Cache for memoisation */
static seq_t *cache = NULL;        /* dynamic array for computed values   */
static idx_t cache_cap = 0;        /* current capacity of cache array     */
static idx_t cache_len = 0;        /* number of computed entries          */

/* Ensure that the cache can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
    while (new_cap <= n)
        new_cap *= 2;

    size_t alloc_size = (size_t)new_cap * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, alloc_size);
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function that returns a(n). */
seq_t a(idx_t n)
{
    /* Handle the trivial base cases immediately. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* Prepare the cache if this is the first call. */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Grow the cache to accommodate up to index n. */
    ensure_cache(n);

    /* Iterate from the first uncomputed index up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t ni = (seq_t)i;
        seq_t coeff1 = (seq_t)6 * ni - (seq_t)7;
        seq_t coeff2 = (seq_t)8 * ((seq_t)(i - 1)) * ((seq_t)(i - 2));

        seq_t term1 = coeff1 * cache[i - 1];
        seq_t term2 = coeff2 * cache[i - 2];
        cache[i] = -(term1 + term2);            /* solve recurrence */
    }

    cache_len = n + 1;
    return cache[n];
}
