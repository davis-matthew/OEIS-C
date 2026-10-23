/*
 * A014679  G.f.: (1+x^3)^2/((1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A014679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 885, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=1, a(3)=3, a(4)=2, a(5)=3, a(6)=6,
   a(n)= 2*a(n-1)-2*a(n-2)+3*a(n-3)-3*a(n-4)+2*a(n-5)-2*a(n-6)+a(n-7) for n≥7 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoisation buffer               */
static idx_t  cache_size = 0;        /* allocated capacity                */
static idx_t  cache_used  = 0;        /* number of terms currently stored   */

seq_t a(idx_t n)
{
    /* ensure the buffer can hold indices up to n */
    if (n >= cache_size) {
        idx_t new_size = cache_size ? cache_size : 8;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                          /* allocation failure – unlikely */
            cache = NULL;
            cache_size = 0;
            cache_used = 0;
            return 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise the first known values once */
    if (cache_used == 0) {
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 2;
        cache[5] = 3;
        cache[6] = 6;
        cache_used = 7;                      /* indices 0–6 are available */
    }

    /* if the requested term is already computed, return it */
    if (n < cache_used) {
        return cache[n];
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_used; i <= n; ++i) {
        /* all indices i-1 … i-7 are valid at this point */
        seq_t val = 2 * cache[i-1] - 2 * cache[i-2]
                    + 3 * cache[i-3] - 3 * cache[i-4]
                    + 2 * cache[i-5] - 2 * cache[i-6]
                    +  cache[i-7];
        cache[i] = val;
    }

    cache_used = n + 1;
    return cache[n];
}
