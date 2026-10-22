/*
 * A003000  Number of bifix-free (or primary, or unbordered) words of length n
 *          over a two-letter alphabet.
 * https://oeis.org/A003000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) - [n even]*a(n/2), a(0) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/A_grow.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_filled = 0;

seq_t a(idx_t n) {
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        if (new_cap < 16) new_cap = 16;
        cache = A_grow(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
    if (cache_filled == 0) {
        cache[0] = (seq_t)1;
        cache_filled = 1;
    }
    while (cache_filled <= n) {
        idx_t i = cache_filled;
        seq_t v = cache[i - 1] * 2;
        if (i % 2 == 0) {
            v -= cache[i / 2];
        }
        cache[i] = v;
        cache_filled++;
    }
    return cache[n];
}
