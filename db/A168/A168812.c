/*
 * A168812  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A168812
 *
 *  a(0) = 1
 *  for n ≥ 1:
 *      let p(n) = 1 if n == 0,
 *                  2 if 1 ≤ n ≤ 18,
 *                  1 if n == 19,
 *                  0 otherwise.
 *      a(n) = 37·(a(n-1)+a(n-2)+…+a(n-18))
 *             – 703·a(n-19) + p(n)
 *
 *  The recurrence follows from the generating function
 *  (t^19 + 2t^18 + … + 2t + 1)/(1 – 37t – 37t^2 – … – 37t^18 + 703t^19).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[0..cache_last] */
    static idx_t   cache_last = (idx_t)-1; /* last index computed */

    /* --- initialisation ------------------------------------------------ */
    if (cache_last == (idx_t)-1) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;                  /* a(0) = 1      */
        cache_last = 0;
    }

    /* --- extend cache if necessary ------------------------------------- */
    if (n > cache_last) {
        /* allocate enough space for [0 .. n] */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t sum18 = 0;
            /* sum of a(i-1) .. a(i-18) */
            for (idx_t j = 1; j <= 18; ++j) {
                if (i >= j)
                    sum18 += cache[i - j];
            }

            seq_t val = sum18 * (seq_t)37;
            if (i >= 19)
                val -= cache[i - 19] * (seq_t)703;

            /* add the numerator contribution p(i) */
            if (i == 0)
                val += (seq_t)1;
            else if (i <= 18)
                val += (seq_t)2;
            else if (i == 19)
                val += (seq_t)1;

            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[n];
}
