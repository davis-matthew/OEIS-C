/*
 * A021446  Decimal expansion of 1/442.
 * https://oeis.org/A021446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 442) mod 10 (the n-th decimal digit of
 *            1/442)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *digit_cache = NULL;       /* cached digits of 1/442 */
static size_t cache_len = 0;            /* number of digits cached */
static size_t cache_cap = 0;            /* allocated capacity of digit_cache */
static uint64_t rem          = 1;        /* remainder in the continued division */

/* extend cache to include the nth digit (if not already present) */
static void ensure_cache_size(idx_t n)
{
    /* n is a nonnegative 64‑bit integer; here we assume it fits into size_t */
    size_t target = (size_t)n + 1;
    if (target <= cache_len) return;

    /* grow capacity if necessary */
    if (target > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
        digit_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new digits up to target-1 */
    for (size_t i = cache_len; i < target; ++i) {
        uint64_t digit = (rem * 10ULL) / 442ULL;
        rem = (rem * 10ULL) % 442ULL;
        digit_cache[i] = (seq_t)digit;
    }
    cache_len = target;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return digit_cache[(size_t)n];
}
