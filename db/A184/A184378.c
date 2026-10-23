/*
 * A184378  1/12 the number of (n+2) X 3 0..2 arrays with each 3 X 3 subblock
 *          containing three of each value.
 * https://oeis.org/A184378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 27*a(n-2) + 81*a(n-3) - 630*a(n-4) +
 *            1701*a(n-5) - 1620*a(n-6) + 4860*a(n-7) - 13122*a(n-8) +
 *            8748*a(n-9) Initial values: a(1)=140, a(2)=545, a(3)=2174,
 *            a(4)=9113, a(5)=43295, a(6)=212747, a(7)=1096781, a(8)=6045233,
 *            a(9)=33958643
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
static idx_t cache_len = 0;     /* number of terms currently cached */
static idx_t cache_cap = 0;     /* allocated capacity */

static const seq_t init[9] = {
    140ULL, 545ULL, 2174ULL, 9113ULL, 43295ULL,
    212747ULL, 1096781ULL, 6045233ULL, 33958643ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1 */

    /* Expand storage if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to recompute without caching */
            free(cache);
            cache = NULL;
            cache_cap = cache_len = 0;
            /* Will recompute below without caching */
        } else {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute any missing terms */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 9) {
                val = init[i - 1];
            } else {
                /* Positive contributions */
                seq_t pos = mul_small(cache[i - 2], 10);
                pos += mul_small(cache[i - 4], 81);
                pos += mul_small(cache[i - 6], 1701);
                pos += mul_small(cache[i - 8], 4860);
                pos += mul_small(cache[i -10], 8748);

                /* Negative contributions */
                seq_t neg = mul_small(cache[i - 3], 27);
                neg += mul_small(cache[i - 5], 630);
                neg += mul_small(cache[i - 7], 1620);
                neg += mul_small(cache[i - 9], 13122);

                val = pos - neg;
            }

            if (cache) /* only write back when cache exists */
                cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache ? cache[n - 1] : init[0];  /* if cache is NULL, return a(1) */
}
