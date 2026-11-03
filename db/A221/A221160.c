/*
 * A221160  G.f.: Sum_{n>=0} (4*n+1)^n * x^n / (1 + (4*n+1)*x)^n.
 * https://oeis.org/A221160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+3) * 4^(n-1) * n! for n>0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

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

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* n! */
    seq_t fact = factorial(n);

    /* 4^(n-1) */
    seq_t pow4 = ipow_idx((seq_t)4, n - 1);

    /* (2*n+3) */
    seq_t mult_const = (seq_t)(2 * n + 3);

    /* Compute the product in stages to avoid intermediate overflow for
     * small seq_t types – the values we test will fit in seq_t. */
    seq_t tmp = fact * pow4;          /* (n! * 4^(n-1))   */
    seq_t result = tmp * mult_const; /* (2*n+3) * 4^(n-1) * n! */

    return result;
}
