/*
 * A016177  a(n) = 8^n - 7^n.
 * https://oeis.org/A016177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 7^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 8^n and 7^n using the provided big_pow routine
       and return their difference. */
    seq_t p8 = big_pow((seq_t)8, (seq_t)n);
    seq_t p7 = big_pow((seq_t)7, (seq_t)n);
    return p8 - p7;
}
