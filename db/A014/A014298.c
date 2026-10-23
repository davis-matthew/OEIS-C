/*
 * A014298  a(n) = 2^n * (3n)! / (2n+1)!.
 * https://oeis.org/A014298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (3n)! / (2n+1)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = 2^n * ∏_{i=2n+2}^{3n} i */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Use the power-of-two utility first. */
    seq_t res = pow2(n);

    /* Multiply by each integer in the range [2n+2, 3n]. */
    idx_t start = 2 * n + 2;
    idx_t end   = 3 * n;
    for (idx_t i = start; i <= end; ++i)
        res *= i;

    return res;
}
