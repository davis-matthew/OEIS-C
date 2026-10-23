/*
 * A169152  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 25: a(n) = 2 + 41 * Σ_{k=1}^{n} a(n-k) For
 *            n = 26: a(26) = 1 + 41 * Σ_{k=1}^{25} a(26-k) – 861 * a(0) For n
 *            > 26: a(n) = 41 * Σ_{k=1}^{25} a(n-k) – 861 * a(n-26) This
 *            follows from the rational generating function shown in the OEIS
 *            entry: (t^26 + 2t^25 + … + 2t + 1) / (861 t^2...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;      /* dynamic array */
    static idx_t  cached = 0;        /* cache size = number of computed terms */

    /* Ensure cache large enough for the requested index */
    if (cached <= n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort with NULL? */
            /* In an environment where errno handling is required,
               one might set errno and return 0.  Here we simply
               terminate to keep the implementation straightforward. */
            abort();
        }
        cache  = new_cache;
        /* Compute missing terms one by one */
        for (; cached <= n; ++cached) {
            if (cached == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            /* Sum of the previous at most 25 terms */
            seq_t sum = 0;
            idx_t i;
            for (i = 1; i <= 25 && cached >= i; ++i)
                sum += cache[cached - i];

            if (cached == 26) {
                /* a(26) uses a(0) with coefficient 861 and constant 1 */
                cache[26] = (seq_t)1 + (seq_t)41 * sum - (seq_t)861 * cache[0];
            } else if (cached > 26) {
                cache[cached] = (seq_t)41 * sum - (seq_t)861 * cache[cached - 26];
            } else {
                /* Cached ≤ 25 : constant term 2 */
                cache[cached] = (seq_t)2 + (seq_t)41 * sum;
            }
        }
    }
    return cache[n];
}
