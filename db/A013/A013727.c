/*
 * A013727  a(n) = 22^(2*n + 1).
 * https://oeis.org/A013727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t exp = 2 * n + 1;          /* exponent 2*n+1 */
    return seq_pow((seq_t)22, exp); /* compute 22^exp */
}
