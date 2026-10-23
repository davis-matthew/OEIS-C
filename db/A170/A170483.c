/*
 * A170483  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170483
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

/*  OEIS A170483 (Number of reduced words of length n in the Coxeter group on 42 generators)
 *  Recurrence derived from the rational generating function:
 *      G(t) = (t^45 + 2*t^44 + ... + 2*t + 1)/(1 - 40*t - 40*t^2 - ... - 40*t^44 + 820*t^45)
 *
 *  The recurrence is:
 *      a(0) = 1
 *      a(n) = p(n) + 40 * Σ_{k=1}^{min(44,n)} a(n-k) - (n>=45 ? 820*a(n-45) : 0)
 *
 *  where
 *      p(0) = 1,
 *      p(1..44) = 2,
 *      p(45) = 1,
 *      p(n>45) = 0.
 *
 *  This function computes a(n) for any n≥0, memoizing values in a static cache
 *  so calls with arbitrary ordering remain efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        /* realloc or allocate a larger array; cast needed for C++ compatibility */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) /* allocation failed */
            return 0;   /* or abort; behaviour unspecified for OOM */
        cache = new_cache;

        /* compute values that are not yet computed */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t coeff_p;
            if (i == 0)
                coeff_p = 1;
            else if (i <= 44)
                coeff_p = 2;
            else if (i == 45)
                coeff_p = 1;
            else
                coeff_p = 0;

            seq_t sum = 0;
            /* sum of the last up to 44 terms */
            idx_t start = (i >= 45) ? i - 44 : 0;
            for (idx_t j = start; j < i; ++j)
                sum += cache[j];

            seq_t term = coeff_p + (seq_t)40 * sum;
            if (i >= 45) {
                term -= (seq_t)820 * cache[i - 45];
            }
            cache[i] = term;
        }

        cache_len = new_len;
    }

    return cache[n];
}
