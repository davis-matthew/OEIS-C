/*
 * A013788  a(n) = 8^(4*n + 1).
 * https://oeis.org/A013788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(n) = 8 * 4096^n, where 4096 = 8^4 */
    return ((seq_t)8) * seq_pow((seq_t)4096, n);
}
