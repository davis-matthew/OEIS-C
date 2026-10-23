/*
 * A164687  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            41·a(n-1)+41·a(n-2)+41·a(n-3)+41·a(n-4)+41·a(n-5)+41·a(n-6)
 *            -861·a(n-7) for n ≥ 7 Initial values: a(0)=1, a(1)=43,
 *            a(2)=1806, a(3)=75852, a(4)=3185784, a(5)=133802928,
 *            a(6)=5619722976 The recurrence follows from the rational
 *            generating function (t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 +
 *            2*t^2...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
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
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of computed terms */

    /* enlarge storage if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed; return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first seven values once */
    if (len == 0) {
        const seq_t init[7] = {
            (seq_t)1,          /* a(0) */
            (seq_t)43,         /* a(1) */
            (seq_t)1806,       /* a(2) */
            (seq_t)75852,      /* a(3) */
            (seq_t)3185784,    /* a(4) */
            (seq_t)133802928,  /* a(5) */
            (seq_t)5619722976  /* a(6) */
        };
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        len = 7;
    }

    if (n < len) return cache[(size_t)n];

    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum6 = cache[i-1] + cache[i-2] + cache[i-3] +
                     cache[i-4] + cache[i-5] + cache[i-6];
        seq_t val = (seq_t)(41) * sum6 - (seq_t)(861) * cache[i-7];
        if (i == 7) val += (seq_t)1;   /* inhomogeneous term for n = 7 */
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
