/*
 * A026244  a(n) = 4^n*(4^n+1)/2.
 * https://oeis.org/A026244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(4^n+1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* 4^n = 2^(2*n) .  For n > 0, 2^(2*n) is even, so we can avoid
       overflow in the product by factoring a division by 2 first. */
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Compute 2^(2*n) using the generic small-rank power function. */
    seq_t pow2n = pow_seq((seq_t)2, (int)(2*n));

    /* For n > 0, pow2n is even; perform the division before the
       multiplication to keep the intermediate result within the
       representable range of seq_t. */
    seq_t half = pow2n / (seq_t)2;
    return half * (pow2n + (seq_t)1);
}
