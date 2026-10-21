/*
 * A077911  Expansion of 1/((1-x)*(1+x+2*x^2-x^3)).
 * https://oeis.org/A077911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   1/((1‑x)*(1+x+2x²‑x³)) Recurrence: a(n) + a(n‑2) – 3·a(n‑3) +
 *            a(n‑4) = 0 for n ≥ 4 Initial values: a(0)=1, a(1)=0, a(2)=‑1,
 *            a(3)=3
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 47, i64 n <= 94, i128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static buffer for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t sz  = 0;   /* number of values currently initialised (0 .. sz-1) */

    /* ensure the buffer can hold indices up to n */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        while (new_cap <= cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* fill missing values up to n */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        if (i == 0)        cache[0] = (seq_t)1;
        else if (i == 1)   cache[1] = (seq_t)0;
        else if (i == 2)   cache[2] = (seq_t)-1;
        else if (i == 3)   cache[3] = (seq_t)3;
        else
            /* a(i) = –a(i‑2) + 3·a(i‑3) – a(i‑4)  (derived from the recurrence) */
            cache[i] = -(cache[i - 2]) + (seq_t)3 * cache[i - 3] - cache[i - 4];
    }
    sz = (size_t)n + 1;

    return cache[(size_t)n];
}
