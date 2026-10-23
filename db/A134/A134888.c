/*
 * A134888  E_8 numbers: a(n) = 2^(2*n) * 3^(3*n) * 5^n * 839^n. (Constants
 *          are prime numbers).
 * https://oeis.org/A134888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 453060^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    return seq_pow((seq_t)453060, n);
}
