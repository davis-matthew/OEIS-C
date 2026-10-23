/*
 * A041600  Numerators of continued fraction convergents to sqrt(318).
 * https://oeis.org/A041600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A041600 – numerators of continued‑fraction convergents to sqrt(318).
   Continued fraction: [17; (1,4,1,34) repeating].
   Let a(0)=17, a(1)=18 and for n≥2
       a(n)=c(n)*a(n‑1)+a(n‑2),
   where the coefficients repeat 1,4,1,34.  The function uses a static
   cache so that any order of calls requires at most linear work.
*/

#include <stdlib.h>
#include <stdint.h>

/* static cache for the numerators */
static seq_t *cache     = NULL;   /* array of numerators */
static size_t cache_cap = 0;      /* allocated capacity (in elements) */
static idx_t  cache_len = 0;      /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Initialise cache with the first term a(0)=17 */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)17;
        cache_len = 1;
    }

    /* If the requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Increase capacity if necessary */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < needed)
            new_cap = needed;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t coef;
        /* First partial quotient (i==1) is 1, others repeat 1,4,1,34 */
        if (i == 1) {
            coef = (seq_t)1;
        } else {
            int r = (int)((i - 1) & 3);   /* periodicity 4 */
            if (r == 0)
                coef = (seq_t)1;
            else if (r == 1)
                coef = (seq_t)4;
            else if (r == 2)
                coef = (seq_t)1;
            else
                coef = (seq_t)34;
        }

        seq_t val;
        if (i == 1) {
            /* a(1) = coef*a(0) + a(-1), with a(-1)=1 */
            val = coef * cache[0] + (seq_t)1;
        } else {
            val = coef * cache[i - 1] + cache[i - 2];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
