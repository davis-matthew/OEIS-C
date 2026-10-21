/*
 * A237456  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          lower median of every 2 X 2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 136*a(n-4) + 832*a(n-6) + 1600*a(n-8) + 1024*a(n-10)
 *            Initial values (offset 1): a(1)=81, a(2)=308, a(3)=1144,
 *            a(4)=4272, a(5)=16280, a(6)=61152, a(7)=231680, a(8)=869120,
 *            a(9)=3299584, a(10)=12382208
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of allocated slots (cache[0] unused) */

    /* at least room for the first 10 terms */
    size_t needed = (size_t)n + 1;
    if (needed < 11)
        needed = 11;

    /* allocate / resize the cache */
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        for (size_t i = 0; i < needed; ++i)
            cache[i] = (seq_t)0;

        /* initial values */
        const seq_t init[10] = {
            (seq_t)81ULL,
            (seq_t)308ULL,
            (seq_t)1144ULL,
            (seq_t)4272ULL,
            (seq_t)16280ULL,
            (seq_t)61152ULL,
            (seq_t)231680ULL,
            (seq_t)869120ULL,
            (seq_t)3299584ULL,
            (seq_t)12382208ULL
        };
        for (size_t i = 1; i <= 10; ++i)
            cache[i] = init[i-1];
        cap = needed;
    } else if (needed > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        for (size_t i = cap; i < needed; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap = needed;
    }

    /* compute missing values up to n */
    if (n > 10) {
        for (idx_t i = 11; i <= n; ++i) {
            if (cache[(size_t)i] != (seq_t)0)
                continue;   /* already computed */
            seq_t val = (seq_t)136 * cache[(size_t)(i-4)] +
                        (seq_t)832 * cache[(size_t)(i-6)] +
                        (seq_t)1600 * cache[(size_t)(i-8)] +
                        (seq_t)1024 * cache[(size_t)(i-10)];
            cache[(size_t)i] = val;
        }
    }

    return cache[(size_t)n];
}
