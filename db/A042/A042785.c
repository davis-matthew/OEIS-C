/*
 * A042785  Denominators of continued fraction convergents to sqrt(923).
 * https://oeis.org/A042785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1276*a(n-6) - a(n-12) for n > 11, with initial values
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=8, a(5)=21, a(6)=1268,
 *            a(7)=2557, a(8)=3825, a(9)=6382, a(10)=10207, a(11)=26796
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* first 12 terms, used for n < 12 and to bootstrap the cache */
static const seq_t init[12] = {
    1, 2, 3, 5, 8, 21, 1268, 2557, 3825, 6382, 10207, 26796
};

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of slots currently allocated */
static size_t max_computed = 0; /* highest index whose value is in cache */

/* grow the cache buffer so it can hold at least 'needed' entries */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_size)
        return;

    size_t new_size = needed + 64;   /* small extra slack */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Special case: first 12 terms are known */
    if (n < 12)
        return init[(size_t)n];

    size_t idx = (size_t)n;

    /* First allocation and bootstrap of the cache if needed */
    if (cache == NULL) {
        ensure_capacity(idx + 1);
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        max_computed = 11;
    }
    /* If the desired index is beyond current allocation, grow it */
    else if (idx >= cache_size) {
        ensure_capacity(idx + 1);
    }

    /* Compute missing terms up to idx */
    if (idx > max_computed) {
        for (size_t i = max_computed + 1; i <= idx; ++i) {
            /* use seq_t arithmetic throughout */
            seq_t term = ( (seq_t)1276 * cache[i - 6] ) - cache[i - 12];
            cache[i] = term;
        }
        max_computed = idx;
    }

    return cache[idx];
}
