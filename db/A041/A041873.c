/*
 * A041873  Denominators of continued fraction convergents to sqrt(458).
 * https://oeis.org/A041873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 214*a(n-3) + a(n-6) for n>5 a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=212, a(4)=429, a(5)=1070
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

static const seq_t base_terms[6] = {1, 2, 5, 212, 429, 1070};

seq_t a(idx_t n) {
    /* Grow the cache if needed and compute missing terms */
    if (n >= cache_size) {
        size_t start = cache_size;
        /* Allocate up to n+1 elements */
        grow_seq_array(&cache, &cache_size, n + 1);
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 5)
                cache[i] = base_terms[i];
            else
                cache[i] = 214 * cache[i - 3] + cache[i - 6];
        }
        cache_size = n + 1;
    }
    return cache[n];
}
