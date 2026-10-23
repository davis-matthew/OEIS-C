/*
 * A140320  a(n) = A137576((3^n-1)/2).
 * https://oeis.org/A140320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*3^(n-1) + 1 (conjectured but holds for all known
 *            terms)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Compute 3^(n-1) using the provided pow3 helper. */
    seq_t pow = pow3(n - 1);

    /* Multiply by 2*n (cast to seq_t to avoid overflow in idx_t). */
    seq_t prod = pow * (((seq_t)2) * ((seq_t)n));

    /* Add the constant 1. */
    return prod + (seq_t)1;
}
