/*
 * A236323  Number of (n+1) X (1+1) 0..1 arrays colored with the sum of the
 *          upper and lower median values of each 2 X 2 subblock.
 * https://oeis.org/A236323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 8*a(n-4) for n ≥ 5 Initial values:
 *            a(1)=16, a(2)=32, a(3)=60, a(4)=120 The function implements
 *            memoised iterative computation
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array indexed 1..n */
static size_t cache_size = 0;    /* allocated size (elements 0..cache_size-1) */
static size_t cache_last = 0;    /* highest index that has a value */

/* Initial values for 1..4 */
static const seq_t init_vals[5] = { (seq_t)0, (seq_t)16, (seq_t)32, (seq_t)60, (seq_t)120 };

seq_t a(idx_t n)
{
    /* undefined for n < 1 – return 0 */
    if ((idx_t)0 == n) return (seq_t)0;

    size_t idx = (size_t)n;

    /* expand the cache if necessary */
    if (cache_size <= idx) {
        size_t new_size = idx + 1;          /* indices 0..idx */
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = (seq_t *)tmp;
        cache_size = new_size;
    }

    /* initialise first four values if they are not yet stored */
    if (cache_last < 4) {
        for (size_t i = cache_last + 1; i <= 4 && i <= idx; ++i) {
            cache[i] = init_vals[i];
        }
        cache_last = (cache_last < 4 ? 4 : cache_last);
    }

    /* compute any missing values up to idx */
    for (size_t i = cache_last + 1; i <= idx; ++i) {
        if (i <= 4) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i - 2]
                     + (seq_t)2 * cache[i - 3]
                     + (seq_t)8 * cache[i - 4];
        }
    }
    cache_last = idx;

    return cache[idx];
}
