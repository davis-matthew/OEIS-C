/*
 * A041333  Denominators of continued fraction convergents to sqrt(180).
 * https://oeis.org/A041333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 322*a(n-4) - a(n-8) Initial values: a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=12, a(4)=317, a(5)=646, a(6)=1609, a(7)=3864
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    /* Base values for n = 0..7 */
    static const seq_t base[8] = {
        (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)12,
        (seq_t)317, (seq_t)646, (seq_t)1609, (seq_t)3864
    };

    /* Ensure the cache can store indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialize base values (safe to overwrite if already present) */
    for (idx_t i = 0; i < 8 && i <= n; ++i) {
        cache[i] = base[i];
    }

    /* Compute terms using the recurrence up to n */
    for (idx_t i = 8; i <= n; ++i) {
        cache[i] = (seq_t)322 * cache[i - 4] - cache[i - 8];
    }

    return cache[n];
}
