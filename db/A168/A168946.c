/*
 * A168946  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168946
   Rational generating function:
     G(t) = (t^22 + 2⋅t^21 + … + 2⋅t + 1)
            ---------------------------------
            378⋅t^22 – 27⋅t^21 – … – 27⋅t + 1

   The sequence satisfies the linear recurrence for n ≥ 22:
     a(n) = 27⋅∑_{k=1}^{21} a(n–k) – 378⋅a(n–22)

   Initial values (n = 0 … 21) are obtained from the numerator:
     a(0) = 1
     a(n) = 2 + 27⋅∑_{j=0}^{n–1} a(j)   for 1 ≤ n ≤ 21
   The first 16 values are listed in the OEIS entry.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* cache[i] = a(i)     */
    static idx_t max_computed = (idx_t)-1; /* largest index for which cache is valid */
    static size_t cache_size = 0; /* allocated capacity */

    /* Ensure cache large enough */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
        if (!tmp) abort();
        cache = tmp;
        cache_size = (size_t)(n + 1);
    }

    /* No values computed yet: initialise a(0) */
    if (max_computed == (idx_t)-1) {
        cache[0] = 1;
        max_computed = 0;
    }

    /* If the requested value is already computed, just return it */
    if (n <= max_computed)
        return cache[n];

    /* ------------------------------------------------------------------
       1. Compute terms 1 … 21 (if not already)
       ------------------------------------------------------------------ */
    if (max_computed < 21) {
        /* cumulative sum of a(0) … a(k) for use in the first 21 values */
        seq_t cum_sum = 0;
        for (idx_t i = 0; i <= max_computed; ++i)
            cum_sum += cache[i];

        for (idx_t k = max_computed + 1; k <= (n < 21 ? n : 21); ++k) {
            seq_t coef = (k == 0) ? (seq_t)1 : (seq_t)2;
            cache[k] = coef + (seq_t)27 * cum_sum;
            cum_sum += cache[k];
        }
        max_computed = (max_computed < 21 ? (n < 21 ? n : 21) : max_computed);
        if (n <= max_computed)
            return cache[n];
    }

    /* ------------------------------------------------------------------
       2. From index 22 onwards, use the recurrence with a sliding window
       ------------------------------------------------------------------ */
    idx_t start = (max_computed < 22) ? 22 : max_computed + 1;

    /* Initialise the window sum: sum of a(start-21) … a(start-1) */
    seq_t window_sum = 0;
    for (idx_t j = start - 21; j < start; ++j)
        window_sum += cache[j];

    for (idx_t i = start; i <= n; ++i) {
        seq_t term1 = window_sum * (seq_t)27;
        seq_t term2 = cache[i - 22] * (seq_t)378;
        cache[i] = term1 - term2;

        /* Update the sliding window for the next term */
        window_sum += cache[i];
        window_sum -= cache[i - 21];
    }

    max_computed = n;
    return cache[n];
}
