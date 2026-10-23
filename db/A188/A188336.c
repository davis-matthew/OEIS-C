/*
 * A188336  Number of nondecreasing arrangements of 6 nonzero numbers in
 *          -(n+4)..(n+4) with sum zero.
 * https://oeis.org/A188336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - a(n-5) + 2*a(n-8) - a(n-11) - a(n-13)
 *            + 2*a(n-15) - a(n-16) Initial terms: a(1)=197, a(2)=424,
 *            a(3)=828, a(4)=1488, a(5)=2519, a(6)=4050, a(7)=6252, a(8)=9314,
 *            a(9)=13479, a(10)=19008, a(11)=26224, a(12)=35472, a(13)=47169,
 *            a(14)=61756, a(15)=79754, a(16)=101712
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 172, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */

    static seq_t *cache = NULL;      /* cached values for a(1)...a(k) */
    static size_t cache_len = 0;     /* number of values stored */

    /* first 16 terms stored in a static array */
    static const seq_t seeds[16] = {
        197ULL, 424ULL, 828ULL, 1488ULL, 2519ULL,
        4050ULL, 6252ULL, 9314ULL, 13479ULL, 19008ULL,
        26224ULL, 35472ULL, 47169ULL, 61756ULL, 79754ULL, 101712ULL
    };

    if (n <= 16) {
        return seeds[n - 1];
    }

    size_t idx_n = (size_t)n;

    /* initialise cache with the first 16 terms on first call */
    if (!cache) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 16; ++i) cache[i] = seeds[i];
        cache_len = 16;
    }

    /* grow storage only if needed */
    if (cache_len < idx_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, idx_n * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        /* cache_len remains the old value until new values are filled */
    }

    /* compute missing terms up to idx_n */
    for (size_t i = cache_len; i < idx_n; ++i) {   /* i == n-1 (0‑based) */
        seq_t a1   = cache[i - 1];   /* a(n-1)  */
        seq_t a3   = cache[i - 3];   /* a(n-3)  */
        seq_t a5   = cache[i - 5];   /* a(n-5)  */
        seq_t a8   = cache[i - 8];   /* a(n-8)  */
        seq_t a11  = cache[i - 11];  /* a(n-11) */
        seq_t a13  = cache[i - 13];  /* a(n-13) */
        seq_t a15  = cache[i - 15];  /* a(n-15) */
        seq_t a16  = cache[i - 16];  /* a(n-16) */

        seq_t pos = a1 + a1;            /* 2*a(n-1) */
        pos = pos + a8 + a8;            /* +2*a(n-8) */
        pos = pos + a15 + a15;          /* +2*a(n-15) */

        seq_t neg = a3 + a5 + a11 + a13 + a16;

        cache[i] = pos - neg;           /* a(n) */
    }

    cache_len = idx_n;   /* now all terms up to n are cached */
    return cache[idx_n - 1];
}
