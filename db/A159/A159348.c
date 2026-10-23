/*
 * A159348  Transform of the finite sequence (1, 0, -1, 0, 1) by the T_{0,0}
 *          transform (see link).
 * https://oeis.org/A159348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n>=7; Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=11, a(5)=24, a(6)=55
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of computed entries in cache */

seq_t a(idx_t n)
{
    /* Ensure the cache has at least the first 7 terms and grows to n+1 */
    if (cache == NULL) {
        size_t init_cap = (n + 1 > 7 ? n + 1 : 7);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory: return 0 (caller handles) */

        /* store the seven known values */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 4;
        cache[4] = 11;
        cache[5] = 24;
        cache[6] = 55;

        cache_len = 7;                  /* we have values 0..6 ready */
    }

    if (n < cache_len)
        return cache[n];

    /* grow cache if needed */
    if (n + 1 > cache_len) {
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory */

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i>=7 because cache_len is at least 7 */
            cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
        }

        cache_len = n + 1;
    }

    return cache[n];
}
