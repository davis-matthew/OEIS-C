/*
 * A074577  a(n) = 6^n + 7^n + 8^n.
 * https://oeis.org/A074577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n + 7^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=21, a(2)=149, a(n)=21*a(n-1)-146*a(n-2)+336*a(n-3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization, works for any call order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first three terms */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)3;   /* a(0) */
        cache[1] = (seq_t)21;  /* a(1) */
        cache[2] = (seq_t)149; /* a(2) */
    }

    /* return cached value if available */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to index n */
    idx_t new_size = n + 1;                     /* maximum index needed + 1 */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    for (idx_t i = cache_size; i <= n; ++i) {
        /* use recurrence: a(i) = 21*a(i-1) - 146*a(i-2) + 336*a(i-3) */
        seq_t t1 = cache[i - 1] * (seq_t)21;    /* 21*a(i-1) */
        seq_t t3 = cache[i - 3] * (seq_t)336;   /* 336*a(i-3) */
        seq_t temp = t1 + t3;                   /* t1 + t3  (avoids underflow) */
        seq_t t2 = cache[i - 2] * (seq_t)146;   /* 146*a(i-2) */
        cache[i] = temp - t2;                   /* final value */
    }

    cache_size = new_size;
    return cache[n];
}
