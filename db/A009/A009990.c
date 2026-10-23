/*
 * A009990  Powers of 46.
 * https://oeis.org/A009990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 46^n (a(0)=1, a(1)=46, a(2)=2116, …)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 46^n using the provided big_pow helper. */
    return big_pow((seq_t)46, (seq_t)n);
}
