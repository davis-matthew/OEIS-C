/*
 * A169506  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + 11 * Σ_{k=1}^{33} a(n-k) – 66 * a(n-34) with b(0)
 *            = 1, b(k) = 2 for 1 ≤ k ≤ 33, b(34) = 1, b(k) = 0 for k > 34
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Convert index to a usable form.  The build limits n to a small range
       (≤ 10 000), so a simple cast to size_t is safe in this context. */
    size_t idx = (size_t)n;

    /* Extend cache if needed, computing missing values. */
    if (idx >= cache_size)
    {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache     = tmp;

        for (size_t i = cache_size; i < new_size; ++i)
        {
            /* Determine the term from the b-part of the recurrence. */
            seq_t bn = 0;
            if (i == 0 || i == 34)
                bn = (seq_t)1;
            else if (i <= 33)
                bn = (seq_t)2;

            /* Compute Σ_{k=1}^{33} a(i-k) */
            seq_t sum33 = 0;
            for (int k = 1; k <= 33 && (size_t)k <= i; ++k)
                sum33 += cache[i - k];

            seq_t term_pos = sum33 * (seq_t)11;

            /* If i ≥ 34, subtract 66 * a(i-34). */
            seq_t term_neg = 0;
            if (i >= 34)
                term_neg = cache[i - 34] * (seq_t)66;

            /* Combine to get a(i) = bn + term_pos – term_neg. */
            seq_t value = bn + term_pos;
            if (term_neg != 0)
                value -= term_neg;

            cache[i] = value;
        }
        cache_size = new_size;
    }

    return cache[n];
}
