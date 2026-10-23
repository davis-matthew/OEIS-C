/*
 * A192427  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=8,
 *            a(n)=a(n-1)+5*a(n-2)-2*a(n-3)-4*a(n-4) (n≥4)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;     /* number of elements currently stored */

    /* If the value is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Need to grow the cache to at least n+1 entries */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)          /* allocation failure – give up */
        return 0;
    cache = tmp;
    cache_len = new_len;

    /* Initialise initial terms on first run or after expanding below 4 */
    if (old_len == 0) {
        if (cache_len > 0) cache[0] = (seq_t)0;
        if (cache_len > 1) cache[1] = (seq_t)1;
        if (cache_len > 2) cache[2] = (seq_t)1;
        if (cache_len > 3) cache[3] = (seq_t)8;
    } else {
        /* Ensure that initial terms are set if cache grew but did not contain them */
        if (old_len <= 1 && cache_len > 1) cache[1] = (seq_t)1;
        if (old_len <= 2 && cache_len > 2) cache[2] = (seq_t)1;
        if (old_len <= 3 && cache_len > 3) cache[3] = (seq_t)8;
    }

    /* Compute any missing values up to n */
    idx_t start = old_len;
    if (start < 4) start = 4;          /* first index that requires the recurrence */

    for (idx_t i = start; i <= n; ++i) {
        seq_t term5 = cache[i - 2] * (seq_t)5;   /* 5*a(i-2) */
        seq_t term2 = cache[i - 3] * (seq_t)2;   /* 2*a(i-3) */
        seq_t term4 = cache[i - 4] * (seq_t)4;   /* 4*a(i-4) */

        seq_t val = cache[i - 1] + term5;  /* a(i-1)+5*a(i-2) */
        val -= term2;                      /* -2*a(i-3) */
        val -= term4;                      /* -4*a(i-4) */

        cache[i] = val;
    }

    return cache[n];
}
