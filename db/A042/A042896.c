/*
 * A042896  Numerators of continued fraction convergents to sqrt(980).
 * https://oeis.org/A042896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 103682*a(n-10) - a(n-20) Initial values: a(0)=31,
 *            a(1)=94, a(2)=313, a(3)=407, a(4)=720, a(5)=1847, a(6)=2567,
 *            a(7)=4414, a(8)=15809, a(9)=51841, a(10)=3229951, a(11)=9741694,
 *            a(12)=32455033, a(13)=42196727, a(14)=74651760, a(15)=191500247,
 *            a(16)=266152007, a(17)=457652254, a(18)=163910876...
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t cache_len = 0;     /* number of terms stored */

    /* initial 20 terms – stored as compile‑time constants */
    static const seq_t init[20] = {
        31, 94, 313, 407, 720,
        1847, 2567, 4414, 15809, 51841,
        3229951, 9741694, 32455033, 42196727,
        74651760, 191500247, 266152007, 457652254,
        1639108769, 5374978561
    };

    /* grow cache if necessary */
    if ((size_t)cache_cap < (size_t)n + 1U) {
        size_t new_cap = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms only */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 20U) {
            cache[i] = init[i];
        } else {
            cache[i] = (seq_t)103682 * cache[i - 10] - cache[i - 20];
        }
    }
    cache_len = n + 1U;

    return cache[n];
}
