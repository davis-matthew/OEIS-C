/*
 * A021441  Decimal expansion of 1/437.
 * https://oeis.org/A021441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/437 at position n
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* computed digits */
static size_t cache_len = 0;         /* number of digits stored */
static size_t cache_cap = 0;         /* capacity of the cache array */

/* rest remainder after the last computed digit (before generating next digit) */
static uint64_t rem = 1;             /* start with numerator 1 for 1/437 */

/* grow the cache so that it can hold at least new_len elements */
static void grow_cache(size_t new_len) {
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* ensure n is not negative (idx_t is unsigned) already */
    if ((size_t)n >= cache_len) {
        size_t target = (size_t)n;
        for (size_t i = cache_len; i <= target; ++i) {
            grow_cache(i + 1);
            uint64_t num   = rem * 10ULL;
            seq_t digit    = (seq_t)(num / 437ULL);
            rem            = num % 437ULL;
            cache[i]       = digit;
        }
        cache_len = target + 1;
    }
    return cache[(size_t)n];
}
