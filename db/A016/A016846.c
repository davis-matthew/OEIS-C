/*
 * A016846  a(n) = (4*n + 3)^10.
 * https://oeis.org/A016846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 20, u128 n <= 1782,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(4 * n + 3);
    return seq_pow(base, 10);   /* compute (4*n+3)^10 */
}
