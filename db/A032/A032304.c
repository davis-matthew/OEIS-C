/*
 * A032304  "EFK" (unordered, size, unlabeled) transform of 1,3,5,7,...
 * https://oeis.org/A032304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Π_{k≥1} (1 + (2*k‑1)*x^k)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 47, u64 n <= 127, u128 n <= 360,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the polynomial coefficients up to the largest n computed */
    static seq_t *dp   = NULL;      /* array of coefficients          */
    static idx_t max_n = 0;         /* largest n for which dp is valid */

    /* Recompute the whole array when a larger n is requested */
    if (dp == NULL || n > max_n)
    {
        /* Allocate or resize the array to hold indices 0..n */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failed */
        dp = tmp;

        /* Clear all entries and set the constant term */
        for (size_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;

        /* Build the product Π_{k=1}^{n} (1 + (2k-1)*x^k) */
        for (idx_t k = 1; k <= n; ++k)
        {
            seq_t coeff = (seq_t)((idx_t)2 * k - 1);   /* 2k-1 */
            for (idx_t j = n; j >= k; --j)
            {
                dp[j] += dp[j - k] * coeff;
            }
        }

        max_n = n;    /* remember new bounds */
    }

    return dp[n];
}
