/*
 * A021710  Decimal expansion of 1/706.
 * https://oeis.org/A021710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-16) + a(n-17) (for n > 17) a(0) = 0, a(1) =
 *            0, a(2) = 1, a(3) = 4, a(4) = 1, a(5) = 6, a(6) = 4, a(7) = 3,
 *            a(8) = 0, a(9) = 5, a(10) = 9, a(11) = 4, a(12) = 9, a(13) = 0,
 *            a(14) = 0, a(15) = 8, a(16) = 4, a(17) = 9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t initial[18] = {
    0, 0, 1, 4, 1, 6, 4, 3, 0, 5,
    9, 4, 9, 0, 0, 8, 4, 9
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    size_t idx = (size_t)n;  /* n is assured to be small (≤ 10000) */

    /* Ensure the cache has room */
    if (cache_cap < idx + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (should not happen for test) */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill missing terms up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 18) {
            cache[i] = initial[i];
        } else {
            /* Compute using recurrence: a(i)=a(i-1)-a(i-16)+a(i-17) */
            int val = (int)cache[i - 1] - (int)cache[i - 16] + (int)cache[i - 17];
            cache[i] = (seq_t)val;
        }
    }
    cache_len = idx + 1;

    return cache[idx];
}
