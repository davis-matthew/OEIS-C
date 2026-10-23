/*
 * A024437  Expansion of 1/((1-x)(1-7x)(1-8x)(1-9x)).
 * https://oeis.org/A024437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - 215*a(n-2) + 695*a(n-3) - 504*a(n-4), a(0)=1,
 *            a(1)=25, a(2)=410, a(3)=5570
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* number of allocated slots */

    /* Small n: return immediately */
    if (n < 4) {
        switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)25;
        case 2: return (seq_t)410;
        case 3: return (seq_t)5570;
        }
    }

    /* Ensure we have enough memory for indices up to n */
    if (n + 1 > cap) {
        size_t need   = (size_t)n + 1;
        size_t new_cap = (need > cap * 2) ? need : cap * 2;
        if (new_cap < 4) new_cap = 4;                 /* at least space for 0..3 */

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                    /* allocation failure */

        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;

        /* initialise first four values if they are missing */
        if (cache[3] == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)25;
            cache[2] = (seq_t)410;
            cache[3] = (seq_t)5570;
        }
    }

    /* The term may already be computed – return it */
    if (cache[n] != 0)
        return cache[n];

    /* Compute missing terms up to n lazily */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        seq_t t = (seq_t)25 * cache[i - 1];
        t += (seq_t)695 * cache[i - 3];
        t -= (seq_t)215 * cache[i - 2];
        t -= (seq_t)504 * cache[i - 4];
        cache[i] = t;
    }
    return cache[n];
}
