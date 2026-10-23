/*
 * A169193  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A169193:
   a(0) = 1.
   For n > 0:
        let p(n) = 1           if n = 0 or 27,
                   2           if 1 ≤ n ≤ 26,
                   0           if n > 27.
        a(n) = p(n) + 34 * Σ_{k=1}^{min(26,n)} a(n-k)
               – (n ≥ 27 ? 595 * a(n-27) : 0).
   This recurrence follows from the rational generating function
   given in the OEIS entry.  The first 27 terms are therefore
   uniquely determined and the same recurrence applies for all n ≥ 27. */

#include <stdlib.h>

/* Compute a(n) for n >= 0.  The function uses a static cache so that
 * any order of calls is efficient.  It works for any integer type
 * seq_t (fixed‑size or arbitrary precision). */
seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* number of terms already computed */

    /* Ensure the cache is large enough for n+1 terms */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap : 64;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory – caller must handle */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;                 /* a(0) = 1 */
            continue;
        }

        /* p(i) : numerator coefficient for the generating function */
        seq_t p_i;
        if (i == 0 || i == 27)
            p_i = (seq_t)1;
        else if (1 <= i && i <= 26)
            p_i = (seq_t)2;
        else
            p_i = (seq_t)0;                     /* for i > 27 */

        /* Sum of the previous at most 26 terms weighted by 34 */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= 26 && k <= i; ++k)
            sum += cache[i - k];

        /* Recurrence */
        seq_t term = p_i + (seq_t)34 * sum;
        if (i >= 27)
            term -= (seq_t)595 * cache[i - 27];

        cache[i] = term;
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
