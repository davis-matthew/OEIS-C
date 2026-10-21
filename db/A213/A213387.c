/*
 * A213387  a(n) = 5*2^(n-1) - 2 - 3*n.
 * https://oeis.org/A213387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n-1) - 2 - 3*n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n-1) using the utility pow2. */
    seq_t v = pow2(n - 1);

    /* Multiply by 5. */
    v = 5 * v;

    /* Subtract the constant 2. */
    v -= 2;

    /* Subtract 3*n. */
    v -= (seq_t)3 * n;

    return v;
}
