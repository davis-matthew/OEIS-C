/*
 * A193418  Expansion of x*(x^2+x-1)/(3*x^6-4*x^5+x^4-3*x^2+4*x-1).
 * https://oeis.org/A193418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-4) - 4*a(n-5) + 3*a(n-6), for n
 *            ≥ 4, with a(0)=0, a(1)=1, a(2)=3, a(3)=8
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 0;            /* last computed index */

    /* initialisation */
    if (cache == NULL) {
        /* allocate at least indices 0..3 */
        cap = (size_t)n + 1;
        if (cap < 4) cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;   /* a(0) = 0 – not part of the sequence but used in the recurrence */
        cache[1] = 1;   /* a(1) = 1 */
        cache[2] = 3;   /* a(2) = 3 */
        cache[3] = 8;   /* a(3) = 8 */
        last = 3;
    }
    else if ((size_t)n + 1 > cap) {
        /* grow cache if needed */
        cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* if already computed return */
    if (n <= last) return cache[n];

    /* compute values up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t val = 4 * cache[i - 1];          /* 4*a(i-1) */
        val -= 3 * cache[i - 2];          /* -3*a(i-2) */
        val +=  cache[i - 4];            /* + a(i-4) */
        if (i >= 5) val -= 4 * cache[i - 5];   /* -4*a(i-5) */
        if (i >= 6) val += 3 * cache[i - 6];   /* +3*a(i-6) */
        cache[i] = val;
    }
    last = (size_t)n;
    return cache[n];
}
