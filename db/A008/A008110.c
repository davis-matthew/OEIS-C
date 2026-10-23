/*
 * A008110  Coordination sequence T1 for Zeolite Code GME and AFX.
 * https://oeis.org/A008110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-5) - a(n-6) + 2*a(n-7) -
 *            2*a(n-8) + a(n-9) - a(n-10) - a(n-12) + a(n-13) - a(n-14) +
 *            a(n-15), for n ≥ 16. Initial values: a(0)=1, a(1)=4, a(2)=9,
 *            a(3)=17, a(4)=29, a(5)=45, a(6)=65, a(7)=89, a(8)=116, a(9)=144,
 *            a(10)=175, a(11)=213, a(12)=256, a(13)=300, a(14...
 * Range:     u8 n <= 11, u16 n <= 193, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[16] = {
    (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)17, (seq_t)29,
    (seq_t)45, (seq_t)65, (seq_t)89, (seq_t)116, (seq_t)144,
    (seq_t)175, (seq_t)213, (seq_t)256, (seq_t)300, (seq_t)346, (seq_t)397
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* fallback – should not happen */
        for (size_t i = 0; i < 16; ++i) cache[i] = init[i];
        cache_len = 16;
    }

    if ((idx_t)n >= cache_len) {
        /* Ensure capacity */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* fallback – should not happen */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms */
        size_t start = cache_len;
        if (start < 16) start = 16;
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t sumPos = 0, sumNeg = 0;
            /* positive coefficients */
            sumPos += cache[i - 1];
            sumPos += cache[i - 3];
            sumPos += cache[i - 5];
            sumPos += cache[i - 7] + cache[i - 7];  /* 2*a[i-7] */
            sumPos += cache[i - 9];
            sumPos += cache[i -13];
            sumPos += cache[i -15];
            /* negative coefficients */
            sumNeg += cache[i - 2];
            sumNeg += cache[i - 6];
            sumNeg += cache[i - 8] + cache[i - 8];  /* 2*a[i-8] */
            sumNeg += cache[i -10];
            sumNeg += cache[i -12];
            sumNeg += cache[i -14];
            cache[i] = sumPos - sumNeg;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
