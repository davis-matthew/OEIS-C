/*
 * A037397  Every base 5 digit of n is a base 10 digit of n.
 * https://oeis.org/A037397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the nth natural number whose every base‑5 digit
   occurs among its decimal digits. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

/* Grow the cache to at least the specified capacity. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < min_cap)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache)
        abort();
    cache_cap = new_cap;
}

/* Test whether every base‑5 digit of x also appears in its decimal
   representation. */
static bool is_valid(seq_t x)
{
    unsigned int dec_mask = 0;
    unsigned int base_mask = 0;
    seq_t t = x;
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 10);
        dec_mask |= 1u << d;
        t /= 10;
    }
    t = x;
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 5);
        base_mask |= 1u << d;
        t /= 5;
    }
    return (base_mask & ~dec_mask) == 0u;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_valid(last_checked)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
