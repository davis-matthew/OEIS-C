/*
 * A052907  Expansion of 1/(1 - 2*x^2 - 2*x^3).
 * https://oeis.org/A052907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-2) + 2*a(n-3),  with a(0)=1, a(1)=0, a(2)=2.
 *  The function memoizes values in a static cache so that calls can be in
 *  arbitrary order.  The cache grows on demand and only the minimal
 *  number of terms is recomputed each time.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms, stored at cache[i] = a(i). */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms stored in cache */
    static idx_t capacity = 0; /* current allocation size */

    /* Initialise cache on first call with the known initial values. */
    if (!cache) {
        capacity = 4;                     /* small start */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return (seq_t)0;       /* out of memory: return zero */
        cache[0] = (seq_t)1;              /* a(0) */
        cache[1] = (seq_t)0;              /* a(1) */
        cache[2] = (seq_t)2;              /* a(2) */
        cached   = 3;                     /* we have 0..2 ready */
    }

    /* Grow cache if the requested index is outside the current capacity. */
    if ((idx_t)(n + 1) > capacity) {
        idx_t newcap = capacity * 2;
        if ((idx_t)(n + 1) > newcap) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;         /* out of memory: return zero */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute any missing terms up to n, using the recurrence. */
    if (n >= 3 && n + 1 > cached) {
        idx_t start = cached;
        if (start < 3) start = 3;          /* start from the first index that
                                             requires the recurrence */
        for (idx_t i = start; i <= n; ++i) {
            seq_t a_i2 = cache[i - 2];
            seq_t a_i3 = cache[i - 3];
            seq_t val  = a_i2 + a_i2;        /* 2*a(i-2) */
            val += a_i3 + a_i3;              /* 2*a(i-3) */
            cache[i] = val;
        }
        if (n + 1 > cached) cached = n + 1;
    }

    /* Return the requested term (guaranteed to be in the cache). */
    return cache[n];
}
