/*
 * A032072  Number of reversible strings with n labeled beads of 5 colors, no
 *          palindromes of more than 1 bead.
 * https://oeis.org/A032072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (5^n - 5^{ceil(n/2)}) / 2 for n ≥ 2 a(1) = 5 (where
 *            ceil(n/2) = (n+1)/2 for unsigned indices)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: compute 5^exp as seq_t */
static seq_t pow5_exp(idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)5;
    return result;
}

/* Helper: compute n! as seq_t */
static seq_t factorial_idx(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but handle n==0 gracefully */
        return (seq_t)0;

    if (n == 1)   /* a(1) = 5 */
        return (seq_t)5;

    seq_t pow5_n        = pow5_exp(n);
    seq_t pow5_half     = pow5_exp((n + 1) / 2);   /* ceil(n/2) */
    seq_t diff          = pow5_n - pow5_half;      /* even difference */
    seq_t half_diff     = diff / 2;                /* safe integer divide */
    seq_t fact          = factorial_idx(n);

    return fact * half_diff;
}
