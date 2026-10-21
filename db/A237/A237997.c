/*
 * A237997  Number of ordered ways to achieve a score of n in American
 *          football taking into account different scoring methods.
 * https://oeis.org/A237997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-6) + a(n-7) + a(n-8), with a(0)=1
 *            and a(k)=0 for k<0. This counts ordered ways to score a football
 *            total using 2,3,6,7,8 point plays
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h> /* for memcpy */
#include "../utils/seq_alloc.h" /* provides seq_alloc(size_t) */

seq_t a(idx_t n)
{
    /* Static cache, shared across calls */
    static seq_t *cache     = NULL; /* array of a[0..cached_n] */
    static idx_t cached_n   = 0;    /* highest index computed so far */
    static size_t cache_cap = 0;    /* allocated size of cache */

    /* Ensure cache is large enough to hold up to n */
    if (n >= cache_cap) {
        /* Allocate new array of appropriate capacity */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = seq_alloc(new_cap);
        /* Copy existing values, if any */
        if (cache != NULL) {
            memcpy(new_cache, cache, (size_t)(cached_n + 1) * sizeof(seq_t));
            /* No need to free old cache: seq_alloc acts as standalone
               allocation; leaks are harmless in contest environment. */
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Initialisation of base cases if cache uninitialised */
    if (cached_n == 0 && cache_cap > 0) {
        if (cache[0] != 1) cache[0] = 1;
        /* Ensure index 1 exists if needed */
        if (n >= 1 && cache[1] != 0) cache[1] = 0;
    }

    /* Extend computation up to n if necessary */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 2) sum += cache[i - 2];
        if (i >= 3) sum += cache[i - 3];
        if (i >= 6) sum += cache[i - 6];
        if (i >= 7) sum += cache[i - 7];
        if (i >= 8) sum += cache[i - 8];
        cache[i] = sum;
    }

    cached_n = n;
    return cache[n];
}
