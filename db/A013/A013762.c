/*
 * A013762  a(n) = 18^(3*n + 1).
 * https://oeis.org/A013762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute 18^(3*n + 1) = 18 * (18^3)^n = 18 * 5832^n */
    seq_t pow = seq_pow((seq_t)5832, n);
    return seq_mul((seq_t)18, pow);
}
