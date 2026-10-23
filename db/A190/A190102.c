/*
 * A190102  Number of 1:3:sqrt(10) proportioned triangles on a (n+1) X (n+1)
 *          grid.
 * https://oeis.org/A190102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-11) - 2*a(n-12) + 2*a(n-14) + a(n-15) - a(n-17) - a(n-19) +
 *            a(n-20) + 2*a(n-22) - 2*a(n-23) - a(n-25) + a(n-26) Initial
 *            values: a(1)=0, a(2)=0, a(3)=24, a(4)=80, a(5)=168, a(6)=344,
 *            a(7)=624, a(8)=1008, a(9)=1584, a(10)=2368, a(...
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   enum_triangles
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This code assumes that seq_t and idx_t are defined by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/enum_triangles.h"

/* Static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_up_to = 0;

/* Ensure the cache can address up to index n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    cache = new_cache;
    cache_cap = needed;
}

/* Compute the sequence term a(n) for 1-based index n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for n < 1 */

    /* Lazy initialization of cache and first 26 terms */
    if (cache == NULL) {
        size_t init_cap = (size_t)((n > 26 ? n : 26) + 1);
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        cache_cap = init_cap;
        for (idx_t i = 1; i <= 26; ++i) {
            cache[i] = enum_triangles(i);
        }
        cached_up_to = 26;
    }

    /* Extend cache if necessary */
    if (n >= cache_cap) {
        ensure_cache_size(n);
    }

    /* Extend values using the recurrence if required */
    if (n > cached_up_to) {
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val += 2 * cache[i - 3];
            val -= 2 * cache[i - 4];
            val -= 1 * cache[i - 6];
            val += 1 * cache[i - 7];
            val += 1 * cache[i - 9];
            val -= 1 * cache[i - 11];
            val -= 2 * cache[i - 12];
            val += 2 * cache[i - 14];
            val += 1 * cache[i - 15];
            val -= 1 * cache[i - 17];
            val -= 1 * cache[i - 19];
            val += 1 * cache[i - 20];
            val += 2 * cache[i - 22];
            val -= 2 * cache[i - 23];
            val -= 1 * cache[i - 25];
            val += 1 * cache[i - 26];
            cache[i] = val;
        }
        cached_up_to = n;
    }

    return cache[n];
}
