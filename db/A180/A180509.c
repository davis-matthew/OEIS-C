/*
 * A180509  Solutions a(n) of (a(n)-5)*(a(n)-6) = 21*b(n)*(b(n)-1).
 * https://oeis.org/A180509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111*a(n-4) - 111*a(n-8) + a(n-12). Initial values
 *            (n=0..11): a(0)=6, a(1)=12, a(2)=26, a(3)=104, a(4)=159,
 *            a(5)=741, a(6)=2267, a(7)=10841, a(8)=16890, a(9)=80904,
 *            a(10)=248750, a(11)=1191812
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;
    static idx_t maxComputed = (idx_t)-1;
    static int initialized = 0;

    /* initialize cache with first 12 terms */
    if (!initialized) {
        cacheSize = 12;
        cache = (seq_t *)malloc(cacheSize * sizeof(seq_t));
        if (!cache) return 0; /* memory error fallback */
        cache[0]  = (seq_t)6;
        cache[1]  = (seq_t)12;
        cache[2]  = (seq_t)26;
        cache[3]  = (seq_t)104;
        cache[4]  = (seq_t)159;
        cache[5]  = (seq_t)741;
        cache[6]  = (seq_t)2267;
        cache[7]  = (seq_t)10841;
        cache[8]  = (seq_t)16890;
        cache[9]  = (seq_t)80904;
        cache[10] = (seq_t)248750;
        cache[11] = (seq_t)1191812;
        maxComputed = 11;
        initialized = 1;
    }

    /* return cached value if already computed */
    if (n <= maxComputed) {
        return cache[n];
    }

    /* grow cache to hold value at index n */
    if ((size_t)n + 1 > cacheSize) {
        size_t newSize = (size_t)n + 1;
        seq_t *newCache = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!newCache) return 0; /* memory error fallback */
        cache = newCache;
        cacheSize = newSize;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t part1 = cache[i - 4] * (seq_t)111;
        seq_t part2 = cache[i - 8] * (seq_t)111;
        seq_t part3 = cache[i - 12];
        seq_t value = part1 + part3;
        value -= part2;               /* ensures non‑negative intermediate */
        cache[i] = value;
    }

    maxComputed = n;
    return cache[n];
}
