/*
 * A162670  Expansion of x/(1 - x - 100*x^2).
 * https://oeis.org/A162670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-2), with a(1)=1, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cache_sz = 0;      /* number of terms currently cached */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* the sequence is not defined for n < 1 */

    size_t need = (size_t)n;

    /* Grow the cache if it does not yet contain the desired term */
    if (cache_sz < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp)
            return 0;           /* allocation failed */

        cache = tmp;

        /* Initialise base cases if the cache was empty */
        if (cache_sz == 0) {
            cache[0] = (seq_t)1;          /* a(1) */
            if (need > 1)
                cache[1] = (seq_t)1;      /* a(2) */
            cache_sz = (need > 1) ? 2 : 1;
        }
        /* If only a(1) was cached, add a(2) */
        else if (cache_sz == 1 && need > 1) {
            cache[1] = (seq_t)1;          /* a(2) */
            cache_sz = 2;
        }

        /* Compute all missing terms up to 'need' */
        for (size_t i = cache_sz; i < need; ++i) {
            /* i corresponds to a(i+1) */
            cache[i] = cache[i-1] + (seq_t)100 * cache[i-2];
        }
        cache_sz = need;
    }

    return cache[n-1];
}
