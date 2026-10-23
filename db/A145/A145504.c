/*
 * A145504  a(n+1) = a(n)^2 + 2*a(n) - 2 and a(1) = 4.
 * https://oeis.org/A145504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 + 2*a(n) - 2, with a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Static cache: cache[0] holds a(1). */
static seq_t *cache   = NULL;
static idx_t cache_size = 0;   /* number of terms currently computed */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* Ensure the cache has capacity for at least `required` terms. */
static void ensure_cache_size(idx_t required)
{
    if (cache_cap < required) {
        idx_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < required) new_cap <<= 1;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = ptr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* a(0) undefined; return 0 for safety */

    /* Grow cache if needed */
    ensure_cache_size(n);

    /* Initialize first term if not yet present */
    if (cache_size == 0) {
        cache[0] = (seq_t)4;               /* a(1) = 4 */
        cache_size = 1;
    }

    /* Compute missing terms up to n */
    while (cache_size < n) {
        seq_t prev = cache[cache_size - 1];
        seq_t val  = prev * prev + (seq_t)2 * prev - (seq_t)2;
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[n - 1];
}
