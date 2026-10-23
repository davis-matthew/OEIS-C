/*
 * A170864  a(0) = 1, a(1) = 1; a(2^i + j) = 3a(j) + 2a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; a(2^i + j) = 3*a(j) + 2*a(j+1) for 0 <= j <
 *            2^i The sequence is generated iteratively, keeping a cache of
 *            previously computed values. This guarantees that any call to
 *            a(n) in any order is answered in O(1) after an initial O(n) fill
 *            up to the maximum requested index
 * Range:     u8 n <= 27, u16 n <= 251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_sz = 0;      /* number of values stored (highest index + 1) */
static size_t cache_cap = 0;     /* allocated capacity of the cache */

/* Return the largest power of two <= x (x > 0). */
static inline idx_t highest_pow2(idx_t x)
{
    idx_t r = 1;
    while (r <= x) r <<= 1;
    return r >> 1;
}

/* Ensure that cache can hold values up to (inclusive) `need`. */
static void ensure_cache(idx_t need)
{
    if (need + 1 <= cache_cap) return;       /* already enough */

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)need) new_cap <<= 1;

    seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) { abort(); }                   /* out of memory, terminate */
    cache = newp;
    cache_cap = new_cap;
}

/* Main sequence function with external linkage. */
seq_t a(idx_t n)
{
    if (n >= cache_sz) {
        /* Compute missing values up to n. */
        ensure_cache(n);
        for (idx_t i = cache_sz; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = 1;
            } else {
                idx_t p = highest_pow2(i);
                idx_t j = i - p;              /* 0 <= j < p */
                cache[i] = (seq_t)3 * cache[j] + (seq_t)2 * cache[j + 1];
            }
        }
        cache_sz = n + 1;
    }
    return cache[n];
}
