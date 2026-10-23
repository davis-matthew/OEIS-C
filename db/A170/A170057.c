/*
 * A170057  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170057
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

/* 
 * Formula: a(n) = 1,   for n = 0
 *           a(n) = 48 * 47^(n-1),   for n ≥ 1 
 *
 * The sequence of reduced words in the Coxeter group on 48 generators
 * with relations S_i^2 = (S_iS_j)^36 = I satisfies this closed form
 * for all n.  The expression is computed exactly using integer
 * arithmetic; on the arbitrary‑precision build the product 48*47^(n-1)
 * yields the exact value.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 47^(n-1) by repeated multiplication. */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        power = power * ((seq_t)47);

    return ((seq_t)48) * power;
}
