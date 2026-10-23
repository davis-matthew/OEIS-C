/*
 * A135914  a(n) = 4*3^n - 2*2^n - 1.
 * https://oeis.org/A135914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^n - 2*2^n - 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^n and 2^n using the provided big_pow routine. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* a(n) = 4*3^n - 2*2^n - 1 */
    return (seq_t)4 * pow3 - (seq_t)2 * pow2 - (seq_t)1;
}
