/*
 * A063097  Dimension of the space of weight 2n cusp forms for Gamma_0( 29 ).
 * https://oeis.org/A063097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-7 + (-1)^n + 10*n) / 2 for n ≥ 2 a(1) = 2 (This is the
 *            dimension of the space of weight 2n cusp forms for Γ₀(29))
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    /* Compute the expression in signed 64‑bit arithmetic to avoid
       underflow when using unsigned types.  For n ≥ 2 the value of
       the numerator is always positive. */
    int64_t nn   = (int64_t)n;
    int64_t pow  = (n & 1ULL) ? (int64_t)-1 : (int64_t)1;  /* (-1)^n */
    int64_t tmp  = -7 + pow + 10 * nn;                    /* numerator */
    return (seq_t)(tmp / 2);                             /* final value */
}
