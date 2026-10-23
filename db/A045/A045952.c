/*
 * A045952  a(n) = 2^(2*n-1)*binomial(2*n,n) + 2^(4*n-1).
 * https://oeis.org/A045952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-1) * binomial(2*n, n) + 2^(4*n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This follows the OEIS definition of sequence A045952.) */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* Compute binomial(2*n, n) */
    seq_t bin = binomial(2 * n, n);

    /* Compute powers of two */
    seq_t p1 = pow2(2 * n - 1);
    seq_t p2 = pow2(4 * n - 1);

    /* Combine according to the formula */
    return p1 * bin + p2;
}
