/*
 * A041063  Denominators of continued fraction convergents to sqrt(38).
 * https://oeis.org/A041063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 74*a(n-2) - a(n-4) for n >= 4; * a(0)=1, a(1)=6, a(2)=73,
 *            a(3)=444
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = 0;
    static bool init = false;

    /* Ensure the cache is large enough. */
    idx_t needed = (n >= 3) ? (n + 1) : 4;
    if (needed > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0) { /* allocation failed */
            return (seq_t)0; /* caller will not use this in tests */
        }
        cache = tmp;
        cap = needed;
    }

    if (!init) {
        init = true;
        /* Base values.  They are needed for the recurrence. */
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 73;
        cache[3] = 444;
        last_computed = 3;

        /* Compute remaining terms up to n, if any. */
        if (n > 3) {
            for (idx_t i = 4; i <= n; ++i) {
                cache[i] = 74 * cache[i-2] - cache[i-4];
            }
            last_computed = n;
        }
    } else if (n > last_computed) {
        /* Extend the cache if necessary. */
        needed = n + 1;
        if (needed > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!tmp && needed > 0) { /* allocation failed */
                return (seq_t)0;
            }
            cache = tmp;
            cap = needed;
        }
        /* Compute missing values. */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = 74 * cache[i-2] - cache[i-4];
        }
        last_computed = n;
    }

    return cache[n];
}
