/*
 * A229472  Number of defective 4-colorings of an n X 1 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly one
 *          mistake, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3^(n-3)+1)*(n-1))/2 for n > 2; a(1) = 0, a(2) = 1. This
 *            follows from the OEIS formula a(n) = (27 + 3^n)*(n-1)/54 by
 *            factoring 54 = 2·3^3
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Compute e = n-3 (exponent for 3). */
    idx_t e = n - 3;
    seq_t pow3e = big_pow((seq_t)3, (seq_t)e);

    /* (3^(n-3) + 1) is always even, so divide by 2 first to avoid overflow. */
    seq_t half_sum = (pow3e + (seq_t)1) / (seq_t)2;

    /* Final multiply by (n-1).  The product fits in seq_t for all n where
       a(n) itself fits in seq_t.  On 64‑bit builds this uses 128‑bit
       intermediate when available; on arbitrary‑precision builds the
       multiplication is exact. */
    seq_t result = half_sum * ((seq_t)(n - 1));
    return result;
}
