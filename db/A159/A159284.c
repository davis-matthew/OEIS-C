/*
 * A159284  Expansion of x*(1+x)/(1-x^2-2*x^3).
 * https://oeis.org/A159284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3), a(0)=0, a(1)=a(2)=1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 107,
 *            u128 n <= 213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently allocated */

    /* Return if already computed. */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Remember old size before reallocation. */
    idx_t old_size = cache_size;

    /* Allocate or grow the cache to hold elements up to index n. */
    size_t new_cap = (size_t)(n + 1);            /* allocation size in bytes */
    if (!cache)
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

    /* Update allocated size. */
    cache_size = new_cap;

    /* Initialize the first three terms if needed. */
    if (cache_size > 0) cache[0] = 0;
    if (cache_size > 1) cache[1] = 1;
    if (cache_size > 2) cache[2] = 1;

    /* Compute missing terms, starting from the first index not yet set. */
    idx_t start = old_size > 3 ? old_size : 3;
    for (idx_t i = start; i <= n; ++i)
        cache[(size_t)i] = cache[(size_t)(i-2)] + cache[(size_t)(i-3)] * 2;

    return cache[(size_t)n];
}
