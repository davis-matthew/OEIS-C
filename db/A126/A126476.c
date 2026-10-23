/*
 * A126476  Number of base 8 n-digit numbers with adjacent digits differing by
 *          three or less.
 * https://oeis.org/A126476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) a(0)=1, a(1)=8,
 *            a(2)=44, a(3)=252, a(4)=1446
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// NOTE: The implementation uses a simple recurrence and a memoised
// static cache.  It works for all integer types that support the
// required operators (e.g. uint8_t…uint128_t or arbitrary‑precision).

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t last = 0;   /* last index that has been computed */
    static size_t cap = 0;    /* allocated capacity */
    static int init = 0;      /* initialization flag */

    /* initialise cache */
    if (!init) {
        cap = 5;  /* store indices 0..4 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)44;
        cache[3] = (seq_t)252;
        cache[4] = (seq_t)1446;
        last = 4;
        init = 1;
    }

    /* return known values for n < 5 */
    if (n <= 4)
        return cache[(size_t)n];

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;  /* double until enough */
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* iteratively compute up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t term = 6 * cache[i-1];          /* 6*a(i-1) */
        term += cache[i-4];                   /* + a(i-4) */
        term -= cache[i-2];                   /* - a(i-2) */
        term -= 3 * cache[i-3];               /* - 3*a(i-3) */
        cache[i] = term;
    }
    last = (size_t)n;
    return cache[(size_t)n];
}
