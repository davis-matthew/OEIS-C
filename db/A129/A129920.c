/*
 * A129920  Expansion of -1/(1 - x + 3*x^2 - 2*x^3 + x^4 - 2*x^5 + x^6).
 * https://oeis.org/A129920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) + 2*a(n-5) -
 *            a(n-6), a(0) = -1, a(1) = -1, a(2) = 2, a(3) = 3, a(4) = -4,
 *            a(5) = -10
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 44, i64 n <= 90, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t cache[10001] = {0};      /* preallocate up to n = 10000 */
static idx_t cache_size = 0;          /* number of cached values (0 => not yet initialized) */

static void ensure_cache(void) {
    if (cache_size) return;
    cache[0] = (seq_t)-1;
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)-4;
    cache[5] = (seq_t)-10;
    cache_size = 6;
}

seq_t a(idx_t n) {
    /* For the given constraints n will be ≤ 10000, so the static array
       covers all required values. */
    ensure_cache();
    if (n < cache_size) return cache[n];
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t t5 = cache[i-5];
        seq_t t6 = cache[i-6];
        cache[i] = t1 - (3 * t2) + (2 * t3) - t4 + (2 * t5) - t6;
    }
    cache_size = n + 1;
    return cache[n];
}
