/*
 * A165270  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165270.
   Recurrence derived from the rational generating function:
     a(n) = 12*(a(n-1)+...+a(n-8)) - 78*a(n-9) + c(n)
   with initial conditions:
     a(0) = 1
     c(0)=1,   c(1…8)=2,   c(9)=1,   c(n)=0 for n≥10
   The recurrence holds for all n≥1. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* memoisation storage */
    static seq_t *cache = NULL;      /* array of computed a(i) */
    static size_t sz    = 0;         /* number of elements stored (no. of terms computed) */
    static size_t cap   = 0;         /* allocated capacity of cache */

    /* grow cache if necessary */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        /* coefficient from the numerator of the generating function */
        seq_t c;
        if (i == 0)
            c = (seq_t)1;
        else if (i <= 8)
            c = (seq_t)2;
        else if (i == 9)
            c = (seq_t)1;
        else
            c = (seq_t)0;

        /* sum of the previous eight terms */
        seq_t sum = (seq_t)0;
        for (size_t j = 1; j <= 8 && i >= j; ++j)
            sum += cache[i - j];

        /* value of the 9‑term‑back term (zero for i < 9) */
        seq_t term9 = (i >= 9) ? cache[i - 9] : (seq_t)0;

        /* recurrence */
        seq_t val = (seq_t)12 * sum;
        if (i >= 9)
            val -= (seq_t)78 * term9;
        val += c;

        cache[i] = val;
    }

    sz = (size_t)(n + 1);
    return cache[n];
}
