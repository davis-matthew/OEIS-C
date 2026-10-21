/*
 * A075676  Sequences A001644 and A000073 interleaved.
 * https://oeis.org/A075676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-2) + a(n-4) + a(n-6); Initial values: a(0)=3,
 *            a(1)=1, a(2)=3, a(3)=2, a(4)=11, a(5)=7
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms */
    static idx_t cache_size = 0;  /* allocated array size */
    static idx_t cache_len  = 0;  /* number of computed terms */

    /* Return immediately if the term is already known */
    if (n < cache_len)
        return cache[n];

    /* Extend backing array if necessary */
    if (cache_size < n + 1) {
        idx_t new_size = cache_size ? cache_size * 2 : 8;
        while (new_size <= n)
            new_size *= 2;
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Initialise the first six terms on the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)7;
        cache_len = 6;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 3 * cache[i - 2] + cache[i - 4] + cache[i - 6];
    }

    /* Update the length of computed terms */
    if (n + 1 > cache_len)
        cache_len = n + 1;

    return cache[n];
}
