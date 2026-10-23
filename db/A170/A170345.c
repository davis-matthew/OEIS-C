/*
 * A170345  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff_N + 46·Σ_{k=1}^{41} a(n‑k) – 1081·a(n‑42), where
 *            coeff_N = 1 if n=0, 2 if 1≤n≤41, 1 if n=42, 0 otherwise. The
 *            recurrence follows from the rational generating function of
 *            A170345
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* persistent cache of computed terms */
    static seq_t *cache    = NULL;   /* array of terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Ensure enough space for all terms up to n (expand geometrically) */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            /* allocation failure – raise an error or abort */
            return (seq_t)0;   /* placeholder; actual error handling can be added */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute terms from cache_len up to n inclusive */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* coeff_N depending on i */
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)1;
        else if (i <= 41)
            coeff = (seq_t)2;
        else if (i == 42)
            coeff = (seq_t)1;
        else
            coeff = (seq_t)0;

        /* Sum over the last 41 terms (only those with non‑negative index) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 41 && k <= i; ++k)
            sum += cache[i - k];

        /* Term a(i‑42) if i >= 42, otherwise 0 */
        seq_t a_i42 = (i >= 42) ? cache[i - 42] : (seq_t)0;

        /* Recurrence: a i = coeff + 46*sum – 1081*a_i42 */
        cache[i] = coeff + (seq_t)46 * sum - (seq_t)1081 * a_i42;
    }

    cache_len = (size_t)(n + 1);
    return cache[n];
}
