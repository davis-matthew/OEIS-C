/*
 * A164391  Number of binary strings of length n with no substrings equal to
 *          0000 or 0111.
 * https://oeis.org/A164391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-5)-a(n-6) for n ≥ 6 initial
 *            values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=25
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise the cache on first use */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)4;   /* a(2) */
        cache[3] = (seq_t)8;   /* a(3) */
        cache[4] = (seq_t)14;  /* a(4) */
        cache[5] = (seq_t)25;  /* a(5) */
        cache_len = 6;
    }

    /* Return if we already have the value */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate up to n */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp) return (seq_t)0;
    cache = tmp;

    /* Compute new values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] + cache[i-2] + cache[i-3];
        val = val - cache[i-5];
        val = val - cache[i-6];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
