/*
 * A215831  a(n) = 3^(-1-floor(n/3))*A215829(n).
 * https://oeis.org/A215831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^(-1-floor(n/3))*A215829(n) Recurrence (derived from the
 *            generating function): a(n) = a(n-9) + 105*a(n-6) - 33*a(n-3) for
 *            n ≥ 9 with initial values a(0)…a(8) as listed in the OEIS entry
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 18, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t cache[10001] = {0};
static idx_t cache_cap = 0;

static void init_cache(void) {
    static const seq_t init[9] = {1, -1, 9, -11, 59, -267, 433, -2041, 9753};
    for (idx_t i = 0; i < 9; ++i)
        cache[i] = init[i];
    cache_cap = 9;
}

seq_t a(idx_t n) {
    /* return the 9 cached initial terms directly */
    const seq_t init[9] = {1, -1, 9, -11, 59, -267, 433, -2041, 9753};
    if (n < 9)
        return init[(idx_t)n];

    if (cache_cap < 9)
        init_cache();

    if (n >= cache_cap) {
        for (idx_t i = cache_cap; i <= n; ++i) {
            /* recurrence: a(i) = a(i-9) + 105*a(i-6) - 33*a(i-3) */
            cache[i] = cache[i - 9] + 105 * cache[i - 6] - 33 * cache[i - 3];
        }
        cache_cap = n + 1;
    }

    return cache[n];
}
