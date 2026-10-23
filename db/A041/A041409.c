/*
 * A041409  Denominators of continued fraction convergents to sqrt(219).
 * https://oeis.org/A041409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 148*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=5, a(4)=144, a(5)=149, a(6)=591, a(7)=740
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_idx = 0;
    static bool initialized = false;

    /* initial values */
    if (!initialized) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – unlikely in tests */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 5;
        cache[4] = 144;
        cache[5] = 149;
        cache[6] = 591;
        cache[7] = 740;
        last_idx = 7;
        initialized = true;
    }

    /* if we already have the value, return it */
    if (n <= last_idx)
        return cache[(size_t)n];

    /* grow cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (n + 1 > new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute forward using recurrence */
    for (idx_t i = last_idx + 1; i <= n; ++i) {
        seq_t val = ((seq_t)148) * cache[(size_t)(i - 4)] - cache[(size_t)(i - 8)];
        cache[(size_t)i] = val;
    }
    last_idx = n;
    return cache[(size_t)n];
}
