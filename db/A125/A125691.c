/*
 * A125691  Expansion of 1/(1 - x - 3x^2 + x^3).
 * https://oeis.org/A125691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) – a(n-3) with a(0) = 1, a(1) = 1, a(2)
 *            = 4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate a static cache that is large enough for n up to ~10 000.
       For larger n the behaviour is undefined, but the specification
       only requires correctness up to about that size. */
    static seq_t cache[10001];
    static idx_t max_cached = 0;
    static bool initialized = false;

    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        max_cached = 2;
        initialized = true;
    }

    if (n <= max_cached)
        return cache[n];

    /* Compute missing terms only once and cache them. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + (cache[i - 2] * (seq_t)3) - cache[i - 3];
    }
    max_cached = n;
    return cache[n];
}
