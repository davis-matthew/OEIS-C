/*
 * A124101  Sum_(x^i*y^j*z^k) with i + j + k = m and (x, y, z) = the primitive
 *          Pythagorean triple (7, 24, 25).
 * https://oeis.org/A124101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 56*a(n-1) - 943*a(n-2) + 4200*a(n-3), a(0) = 1, a(1) =
 *            56, a(2) = 2193
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t cache_size = 0;         /* number of terms in cache [0..cache_size-1] */
static size_t cache_capacity = 0;    /* allocated capacity of cache */

static void ensure_terms(idx_t n)
{
    /* Grow cache storage if needed */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            seq_free_array(cache);
        }
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Initialise base terms if not already */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)56;
        cache[2] = (seq_t)2193;
        cache_size = 3;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = mul_small(cache[i - 3], 4200);
        t1 = t1 + mul_small(cache[i - 1], 56);
        seq_t t2 = mul_small(cache[i - 2], 943);
        cache[i] = t1 - t2;
    }

    if ((idx_t)n + 1 > cache_size) cache_size = (idx_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
