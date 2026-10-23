/*
 * A164332  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 1035*a(n-6) for
 *            n ≥ 7. For n = 6 the numerator of the generating function
 *            contributes an extra +1 term: a(6) =
 *            45*(a(5)+a(4)+a(3)+a(2)+a(1)) - 1035*a(0) + 1. Initial values:
 *            a(0)=1, a(1)=47, a(2)=2162, a(3)=99452, a(4)=4574792,
 *            a(5)=210440432
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 16;                       /* arbitrary small capacity */
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* initial values */
        cache[0] = (seq_t)1ULL;
        cache[1] = (seq_t)47ULL;
        cache[2] = (seq_t)2162ULL;
        cache[3] = (seq_t)99452ULL;
        cache[4] = (seq_t)4574792ULL;
        cache[5] = (seq_t)210440432ULL;
        cache_len = 6;
    }

    /* If the requested term is already cached */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Ensure capacity for all terms up to n */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) {
            new_cap = new_cap ? new_cap * 2 : need;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum5 = cache[i-1];
        sum5 += cache[i-2];
        sum5 += cache[i-3];
        sum5 += cache[i-4];
        sum5 += cache[i-5];

        if (i == 6) {
            /* extra +1 term from the numerator of the G.f. */
            cache[i] = (seq_t)45 * sum5 - (seq_t)1035 * cache[i-6] + (seq_t)1;
        } else {
            cache[i] = (seq_t)45 * sum5 - (seq_t)1035 * cache[i-6];
        }
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
