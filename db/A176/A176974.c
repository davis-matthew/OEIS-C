/*
 * A176974  First exponent n to generate maximum remainder when (a + 1)^n + (a
 *          - 1)^n is divided by a^2 for variable n and a>2.
 * https://oeis.org/A176974
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) (n > 10) Initial values (a(3) … a(10)):
 *            1, 1, 7, 5, 3, 3, 13, 9
 * Range:     u8 n <= 172, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Initial eight terms starting from a(3) */
static const seq_t init_vals[8] =
    { (seq_t)1, (seq_t)1, (seq_t)7, (seq_t)5,
      (seq_t)3, (seq_t)3, (seq_t)13, (seq_t)9 };

/* A static cache big enough for the tested limits (n up to ~10000) */
#define CACHE_MAX 25000
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;   /* number of entries currently valid */

/* Compute a(n) – indexed from 3 */
seq_t a(idx_t n)
{
    /* Below the offset the value is unspecified */
    if (n < 3)
        return (seq_t)0;

    idx_t idx = n - 3;               /* zero‑based position in cache */

    /* Grow the cache on demand (no reallocation: static array) */
    if (idx >= cache_len) {
        /* Ensure we do not exceed the preallocated size */
        if (idx >= CACHE_MAX)
            return (seq_t)0;         /* out of bounds – should not happen */

        /* Fill new entries */
        for (idx_t i = cache_len; i <= idx; ++i) {
            if (i < 8) {
                cache[i] = init_vals[i];
            } else {
                /* recurrence: a(i+3) = 2*a(i-1) - a(i-5) */
                cache[i] = 2 * cache[i - 4] - cache[i - 8];
            }
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
