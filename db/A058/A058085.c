/*
 * A058085  a(n) = A058057(n,n-5).
 * https://oeis.org/A058085
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{k=5..n} (-1)^k * (n-k)! * binomial(k,5) *
 *            binomial(2*n-k,k)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 450 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Compute a(n) for n >= 5. For n < 5 the value is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n < 5) return 0;

    seq_t odd_sum  = 0;   /* sum of terms with odd k (added)   */
    seq_t even_sum = 0;   /* sum of terms with even k (added) */

    for (idx_t k = 5; k <= n; ++k)
    {
        /* (n-k)! */
        seq_t term = factorial(n - k);

        /* * binomial(k,5) */
        term *= binomial(k, 5);

        /* * binomial(2*n-k, k) */
        idx_t two_n_minus_k = 2 * n - k;
        term *= binomial(two_n_minus_k, k);

        if (k & 1)
            odd_sum  += term;   /* k odd -> term counted with + sign in -s   */
        else
            even_sum += term;   /* k even -> term counted with - sign in -s   */
    }

    /* Result: odd_sum - even_sum  (non‑negative for all n >= 5) */
    return odd_sum - even_sum;
}
