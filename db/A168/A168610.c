/*
 * A168610  a(n) = 3^n + 5.
 * https://oeis.org/A168610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^n with the provided arbitrary-precision routine
       and then add 5.  The function works for any seq_t implementation. */
    seq_t val = seq_pow((seq_t)3, n);
    val += (seq_t)5;
    return val;
}
