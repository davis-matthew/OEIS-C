/*
 * A210540  Number of arrays of n nonnegative integers with value i>0
 *          appearing only after i-1 has appeared at least 3 times.
 * https://oeis.org/A210540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n <= 3 else Sum_{i=0}^{n-3} binomial(n-3, i)*a(i)
 *            Initial values: a(0)=a(1)=a(2)=a(3)=1 Implements the recurrence
 *            via a dynamic programming table
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 22, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array of computed a(i) values   */
    static idx_t cap = 0;              /* allocated capacity of cache array      */
    static idx_t last = 3;             /* highest index that has been computed   */

    /* --- initialization, allocate space for first four values --- */
    if (cap < 4) {
        idx_t new_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        /* initialise first four terms to 1 */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cap = new_cap;
    }

    /* --- extend capacity if needed ---------------------------------- */
    if (n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        /* initialise newly allocated entries to zero (they will be filled when computed) */
        for (idx_t i = cap; i < new_cap; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cap = new_cap;
    }

    /* --- trivial base cases ------------------------------------------- */
    if (n <= 3) return 1;

    /* --- compute missing values up to n ------------------------------ */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t m = i - 3;                 /* number of terms in the sum */
        seq_t sum = 0;                   /* running total */
        seq_t binom = 1;                 /* C(m,0) = 1 */

        /* sum_{j=0}^{m} C(m,j) * a(j) */
        for (idx_t j = 0; j <= m; ++j) {
            /* multiply: a(j) * binom */
            seq_t prod = cache[j] * binom;
            sum += prod;

            /* advance binomial coefficient to C(m, j+1) */
            if (j < m) {
                /* binom = binom * (m - j) / (j + 1) */
                seq_t num = binom * (seq_t)(m - j);  /* multiply by small integer */
                seq_t den = (seq_t)(j + 1);          /* divide by small integer */
                binom = num / den;                   /* exact division */
            }
        }
        cache[i] = sum;
    }

    last = n;
    return cache[n];
}
