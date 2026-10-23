/*
 * A159334  Transform of A056594 by the T_{1,1} transformation (see link).
 * https://oeis.org/A159334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+5) = 3*a(n+4) - 3*a(n+3) + 4*a(n+2) - 2*a(n+1) + a(n)
   Initial values: a(0)=2, a(1)=4, a(2)=9, a(3)=23, a(4)=55 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static size_t cache_len = 0;          /* allocated length of cache */
static bool cache_initialized = false;/* whether base values 0..4 are stored */
static idx_t max_computed = 0;        /* highest index that has been computed */

/* Ensure cache has room for indices up to 'needed' (exclusive) */
static void ensure_cache_size(size_t needed)
{
    if (cache_len >= needed)
        return;
    size_t new_len = cache_len ? cache_len * 2 : 8;
    if (new_len < needed)
        new_len = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = new_cache;
    cache_len = new_len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Return the small constants directly for n < 5 */
    if (n < 5) {
        static const seq_t init[5] = { 2, 4, 9, 23, 55 };
        return init[n];
    }

    /* Ensure cache can hold up to n */
    ensure_cache_size((size_t)n + 1);

    /* Initialize cache with the first five terms if not yet done */
    if (!cache_initialized) {
        cache[0] = 2;
        cache[1] = 4;
        cache[2] = 9;
        cache[3] = 23;
        cache[4] = 55;
        cache_initialized = true;
        max_computed = 4;
    }

    /* If requested index already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute terms from max_computed+1 up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t0 = cache[i - 5];
        seq_t t1 = cache[i - 4];
        seq_t t2 = cache[i - 3];
        seq_t t3 = cache[i - 2];
        seq_t t4 = cache[i - 1];
        cache[i] = 3 * t4 - 3 * t3 + 4 * t2 - 2 * t1 + t0;
    }
    max_computed = n;
    return cache[n];
}
