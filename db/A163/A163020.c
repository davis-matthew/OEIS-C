/*
 * A163020  Number of nX3 binary arrays with all 1s connected and a path of 1s
 *          from left column to lower right corner.
 * https://oeis.org/A163020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 33*a(n-2) + 45*a(n-3) - 21*a(n-4) * -
 *            15*a(n-5) + 29*a(n-6) - 9*a(n-7) - 6*a(n-8) + a(n-9) * offset 1,
 *            a(1)=1, a(2)=12, a(3)=82, a(4)=469, a(5)=2502, * a(6)=12966,
 *            a(7)=66325, a(8)=337222, a(9)=1709652
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* initial 9 terms */
    static const seq_t init[9] = {
        1ULL, 12ULL, 82ULL, 469ULL, 2502ULL,
        12966ULL, 66325ULL, 337222ULL, 1709652ULL
    };

    /* For n <= 9 we can answer immediately */
    if (n <= 9) return init[n - 1];

    /* Cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* capacity of the cache */

    /* Initialise cache with the first 9 terms */
    if (!cache) {
        cache_cap = 10;                           /* at least 10 elements */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                      /* allocation failure */
        for (int i = 0; i < 9; ++i) cache[i] = init[i];
        cache_len = 9;
    }

    /* Grow the cache if needed (alloc or realloc) */
    if (n > cache_cap) {
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* If we already have the requested term, return it */
    if (n <= cache_len) return cache[n - 1];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Indices of the required past values in cache */
        seq_t a1 = cache[i - 10];   /* a(n-9)  */
        seq_t a2 = cache[i - 9];    /* a(n-8)  */
        seq_t a3 = cache[i - 8];    /* a(n-7)  */
        seq_t a4 = cache[i - 7];    /* a(n-6)  */
        seq_t a5 = cache[i - 6];    /* a(n-5)  */
        seq_t a6 = cache[i - 5];    /* a(n-4)  */
        seq_t a7 = cache[i - 4];    /* a(n-3)  */
        seq_t a8 = cache[i - 3];    /* a(n-2)  */
        seq_t a9 = cache[i - 2];    /* a(n-1)  */

        /* Accumulate the positive part */
        seq_t pos = a1;
        pos += mul_small(a9, 10);
        pos += mul_small(a7, 45);
        pos += mul_small(a4, 29);

        /* Accumulate the negative part */
        seq_t neg = mul_small(a8, 33);
        neg += mul_small(a6, 21);
        neg += mul_small(a5, 15);
        neg += mul_small(a3, 9);
        neg += mul_small(a2, 6);

        /* Result = pos - neg  (pos >= neg for all n >= 10) */
        seq_t res = pos - neg;

        cache[i - 1] = res;
    }

    cache_len = n;
    return cache[n - 1];
}
