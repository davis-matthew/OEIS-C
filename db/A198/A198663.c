/*
 * A198663  Number of nX2 0..4 arrays with values 0..4 introduced in row major
 *          order and each element equal to one or two horizontal and vertical
 *          neighbors.
 * https://oeis.org/A198663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays with values 0..4 introduced in *
 *            row‑major order such that each element equals one or two of its
 *            * horizontal/vertical neighbors. * * Empirical recurrence (first
 *            order 16) for n>16: * a(n) = 12a(n‑1) – 18a(n‑2) – 96a(n‑3) –
 *            43a(n‑4) + 384a(n‑5) * +1030a(n‑6) +276a(n‑7)...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;      /* 1‑based index: cache[i] = a(i) */
    static idx_t cache_size = 0;     /* allocated size (including unused index 0) */
    static idx_t computed_max = 0;   /* largest n for which cache[i] is valid */

    /* Base sequence values up to n=16 */
    static const seq_t base_vals[17] = {
        0,                         /* 0 (unused, a(0) not defined) */
        1ULL, 3ULL, 13ULL, 65ULL, 401ULL,
        2798ULL, 21243ULL, 170477ULL, 1414186ULL,
        11959573ULL, 102256003ULL, 879709138ULL,
        7594221063ULL, 65683697809ULL, 568711886538ULL,
        4926993076641ULL
    };
    /* Make sure cache is allocated and initialized up to 16 */
    if (!cache || cache_size < 17) {
        if (cache) free(cache);
        cache = (seq_t *)malloc(sizeof(seq_t) * 17);
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = base_vals[i];
        cache_size = 17;
        computed_max = 16;
    }

    /* For small indices: return from base table when needed */
    if (n <= 16)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;   /* 1‑based */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {
            /* Allocation failed; fallback to computing only needed term */
            /* This should not happen for normal inputs. */
            tmp = cache;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms from computed_max+1 up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = cache[i-1] * 12ULL;
        pos += cache[i-5] * 384ULL;
        pos += cache[i-6] * 1030ULL;
        pos += cache[i-7] * 276ULL;
        pos += cache[i-11] * 1080ULL;
        pos += cache[i-12] * 1536ULL;
        pos += cache[i-13] * 768ULL;

        /* Negative contributions */
        seq_t neg = cache[i-2] * 18ULL;
        neg += cache[i-3] * 96ULL;
        neg += cache[i-4] * 43ULL;
        neg += cache[i-8] * 1609ULL;
        neg += cache[i-9] * 3144ULL;
        neg += cache[i-10] * 1688ULL;
        neg += cache[i-14] * 512ULL;

        /* a(i) = pos - neg  (pos >= neg for all valid terms) */
        cache[i] = pos - neg;
    }
    computed_max = n;
    return cache[n];
}
