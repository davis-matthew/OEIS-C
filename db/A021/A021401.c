/*
 * A021401  Decimal expansion of 1/397.
 * https://oeis.org/A021401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/397⌋ mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 1/397 by long division, with caching. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached digits */
static size_t cache_len = 0;         /* number of cached digits */
static size_t cache_cap = 0;         /* cache allocation capacity */
static seq_t  rem = 1;               /* remainder after the last cached digit */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary, then compute any missing digits. */
    if (n >= cache_len) {
        /* Ensure the cache has enough capacity. */
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – return 0 as a fallback */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute new digits starting from the first missing index. */
        seq_t r = rem;
        for (idx_t i = cache_len; i <= n; ++i) {
            r *= (seq_t)10;          /* multiply remainder by 10 */
            cache[i] = r / (seq_t)397;   /* next decimal digit */
            r = r % (seq_t)397;          /* new remainder */
        }
        rem = r;                      /* remainder for next computation */
        cache_len = (size_t)n + 1;    /* update cache length */
    }

    return cache[(size_t)n];
}
