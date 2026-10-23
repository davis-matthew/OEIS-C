/*
 * A167104  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=6, … , a(13)=12288, a(14)=24573; for n>=15:
 *            a(n) = a(n-1)+a(n-2)+…+a(n-13) - a(n-14)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* precomputed initial values up to n=14 */
    static const seq_t init_vals[15] = {
        1ULL,          /* a(0) */
        3ULL,          /* a(1) */
        6ULL,          /* a(2) */
        12ULL,         /* a(3) */
        24ULL,         /* a(4) */
        48ULL,         /* a(5) */
        96ULL,         /* a(6) */
        192ULL,        /* a(7) */
        384ULL,        /* a(8) */
        768ULL,        /* a(9) */
        1536ULL,       /* a(10) */
        3072ULL,       /* a(11) */
        6144ULL,       /* a(12) */
        12288ULL,      /* a(13) */
        24573ULL       /* a(14) */
    };

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 15;                /* need indices 0..14 */
        cache    = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 */
        memcpy(cache, init_vals, cache_len * sizeof(seq_t));
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – return 0 */
        cache = tmp;
        /* compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i <= 14) {
                cache[i] = init_vals[i];
            } else {
                seq_t sum = 0;
                /* sum of last 13 terms */
                for (size_t k = 1; k <= 13; ++k) sum += cache[i - k];
                sum -= cache[i - 14]; /* subtract the 14th term back */
                cache[i] = sum;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
