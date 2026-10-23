/*
 * A120118  a(n) is the number of binary strings of length n such that no
 *          subsequence of length 5 or less contains 3 or more ones.
 * https://oeis.org/A120118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + 2*a(n-5) - a(n-8) - a(n-10); Initial
 *            values: a(0)=1, a(1)=2, a(2)=4, a(3)=7, a(4)=11, a(5)=16,
 *            a(6)=26, a(7)=43, a(8)=71, a(9)=116, a(10)=186
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements allocated (empty if 0) */

    /* initialise cache with the first 11 terms on first call */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;  /* out of memory -> undefined, but keep minimal */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 7;
        cache[4] = 11;
        cache[5] = 16;
        cache[6] = 26;
        cache[7] = 43;
        cache[8] = 71;
        cache[9] = 116;
        cache[10] = 186;
    }

    /* if requested index already computed, return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* need to compute up to n (inclusive) */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (tmp == NULL) return 0;  /* allocation failed */
    cache = tmp;

    for (idx_t i = old_size; i <= n; ++i) {
        seq_t sum = cache[(size_t)(i - 1)]
                 + cache[(size_t)(i - 3)]
                 + 2 * cache[(size_t)(i - 5)];
        sum -= cache[(size_t)(i - 8)];
        sum -= cache[(size_t)(i - 10)];
        cache[(size_t)i] = sum;
    }

    return cache[(size_t)n];
}
