/*
 * A093132  Third binomial transform of Fibonacci(3n+2).
 * https://oeis.org/A093132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*10 - a(n-2)*20, a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache that is extended on demand.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache holding the value of a(k) for k < cache_size          */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first two terms if it is still empty   */
    if (cache_size == 0) {
        /* allocate space for 2 entries, cast required for C compatibility */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0     */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)8;           /* a(1) */
        cache_size = 2;
    }

    /* simple case: the term is already known */
    if (n < cache_size) {
        return cache[(size_t)n];
    }

    /* extend the cache to hold up to a(n) */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;          /* allocation failure – return 0     */
    cache = new_cache;

    /* compute missing terms using recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[(size_t)(i - 1)];
        seq_t prev2 = cache[(size_t)(i - 2)];
        cache[(size_t)i] = (seq_t)10 * prev1 - (seq_t)20 * prev2;
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}
