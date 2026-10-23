/*
 * A188696  Expansion of (1+x^2)/(1-26*x+x^2-26*x^3+2*x^4).
 * https://oeis.org/A188696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1+x^2)/(1-26*x+x^2-26*x^3+2*x^4)
   Recurrence: a(0)=1, a(1)=26, a(2)=676, a(3)=17576,
                for n>=4: a(n)=26*a(n-1)-a(n-2)+26*a(n-3)-2*a(n-4) */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Allocate or grow the cache so that it contains a(idx). */
    if (cache_size == 0) {
        /* First allocation: at least 4 terms for the base cases. */
        size_t needed = idx + 1;
        if (needed < 4) needed = 4;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */

        /* Base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)676;
        cache[3] = (seq_t)17576;

        /* Compute any further terms up to needed‑1. */
        for (size_t i = 4; i < needed; ++i) {
            cache[i] = ((seq_t)26 * cache[i-1]) - cache[i-2]
                       + ((seq_t)26 * cache[i-3]) - ((seq_t)2 * cache[i-4]);
        }
        cache_size = needed;
    } else if (idx >= cache_size) {
        size_t newsize = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;

        /* Compute missing terms. */
        for (size_t i = cache_size; i <= idx; ++i) {
            cache[i] = ((seq_t)26 * cache[i-1]) - cache[i-2]
                       + ((seq_t)26 * cache[i-3]) - ((seq_t)2 * cache[i-4]);
        }
        cache_size = newsize;
    }

    return cache[idx];
}
