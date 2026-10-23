/*
 * A120723  Expansion of
 *          x*(1+3*x)*(1+6*x+16*x^2)/((1-x)*(1+2*x)*(1-3*x-2*x^2)).
 * https://oeis.org/A120723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=11, a(3)=63, a(4)=247; For n>=5:
 *            a(n)=2*a(n-1)+7*a(n-2)-4*a(n-3)-4*a(n-4) (recurrence from G.F.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for 0 is undefined */
    static seq_t *cache = NULL;    /* cache[0] unused, cache[k] holds a(k) */
    static idx_t cached = 0;

    /* initialise cache for the first four terms */
    if (!cache) {
        size_t sz = 5;  /* indices 0..4 */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely in test harness */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 11;
        cache[3] = 63;
        cache[4] = 247;
        cached = 4;
    }

    /* if requested term already computed, return it */
    if (n <= cached) return cache[n];

    /* extend cache to accommodate n */
    size_t new_size = (size_t)n + 1;   /* need indices up to n (0 unused) */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                /* allocation failure – unlikely */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        val *= 2;                         /* 2*a(i-1) */
        val += cache[i-2] * 7;            /* +7*a(i-2) */
        val -= cache[i-3] * 4;            /* -4*a(i-3) */
        val -= cache[i-4] * 4;            /* -4*a(i-4) */
        cache[i] = val;
    }

    cached = n;
    return cache[n];
}
