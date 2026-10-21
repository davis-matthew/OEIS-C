/*
 * A211216  Expansion of
 *          (1-8*x+21*x^2-20*x^3+5*x^4)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A211216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
 *            with initial values a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the linear recurrence directly, caching previously
   computed terms so that any call order is supported. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)5, (seq_t)14 };

    /* static cache to store computed values in order of index */
    static seq_t *cache = NULL;
    static idx_t cached = 0;

    /* provide the first five values without allocation */
    if (n < 5)
        return init[n];

    /* first-time allocation and seeding of the cache */
    if (!cache) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache[4] = init[4];
        cached = 5;
    }

    /* If the requested value is already cached, return it */
    if (n < cached)
        return cache[n];

    /* Grow the cache to contain n+1 terms */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failure */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)9;   /* 9*a(i-1)   */
        seq_t t2 = cache[i - 2] * (seq_t)28;  /* 28*a(i-2)  */
        seq_t t3 = cache[i - 3] * (seq_t)35;  /* 35*a(i-3)  */
        seq_t t4 = cache[i - 4] * (seq_t)15;  /* 15*a(i-4)  */

        /* compute (9*a(i-1) + 35*a(i-3) + a(i-5)) - (28*a(i-2) + 15*a(i-4)) */
        seq_t sum = t1 + t3;
        sum += cache[i - 5];
        seq_t sub = t2 + t4;
        cache[i] = sum - sub;
    }

    cached = n + 1;
    return cache[n];
}
