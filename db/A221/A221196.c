/*
 * A221196  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, with no
 *          occupancy greater than 2.
 * https://oeis.org/A221196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2, a(2) = 10, a(3) = 46 a(n) = 6*a(n-1) - 6*a(n-2) -
 *            2*a(n-3) for n ≥ 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated array size (excluding unused index 0) */
static size_t computed = 0;/* largest index whose value has been computed */

/* grow cache to accommodate at least 'needed' indices (1‑based) */
static void grow_cache(size_t needed)
{
    if (needed <= cap) return;
    size_t newcap = cap < 3 ? 3 : cap * 2;
    while (newcap < needed) newcap <<= 1;
    seq_t *newcache = (seq_t *)malloc((newcap + 1) * sizeof(seq_t));
    if (cache) {
        /* copy existing values, skip unused index 0 */
        memcpy(newcache + 1, cache + 1, cap * sizeof(seq_t));
        free(cache);
    }
    cache = newcache;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; 0 is undefined but safe */

    /* initialize on first call */
    if (computed == 0) {
        grow_cache((size_t)3 > (size_t)n ? 3 : (size_t)n);
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)46;
        computed = 3;
    }

    /* if we have to compute more terms */
    if ((size_t)n > computed) {
        grow_cache((size_t)n);
        for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i-1]
                     - (seq_t)6 * cache[i-2]
                     - (seq_t)2 * cache[i-3];
        }
        computed = (size_t)n;
    }

    return cache[n];
}
