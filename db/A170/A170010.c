/*
 * A170010  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function of A170010.
   G(t) = (t^35 + 2*t^34 + … + 2*t + 1) / (1128*t^35 – 47*t^34 – … – 47*t – 47).
   This gives for n ≥ 35:
       a(n) = 47·(a(n‑1)+…+a(n‑34)) – 1128·a(n‑35)
   plus numerator contributions:
       a(0) = 1,
       a(k) = 2 for 1 ≤ k ≤ 34,
       a(35) receives an extra +1.  The function memoizes results in a
       static cache so each call is O(n). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap    = 0;   /* number of allocated slots          */
    static size_t cached = 0;   /* number of values currently stored */

    /* grow the cache if the requested index is outside the present range */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;                /* allocate up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                           /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing terms */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        seq_t num = 0;

        if (i == 0)          num = 1;                  /* a(0) */
        else if (i <= 34)    num = 2;                  /* 1..34 */
        else if (i == 35)    num = 1;                  /* t^35 term */

        /* sum of the previous up to 34 terms */
        seq_t sum = 0;
        size_t start = (i >= 34) ? i - 34 : 0;
        for (size_t j = start; j < i; ++j)
            sum += cache[j];

        /* apply the recurrence */
        seq_t val = num + 47 * sum;
        if (i >= 35)
            val -= 1128 * cache[i - 35];

        cache[i] = val;
    }

    cached = (size_t)n + 1;
    return cache[(size_t)n];
}
