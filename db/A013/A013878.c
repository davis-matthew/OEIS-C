/*
 * A013878  a(n) = 16^(5*n + 1).
 * https://oeis.org/A013878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(5*n+1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute the exponent e = 5*n + 1 */
    idx_t e = 5*n + 1;
    /* Base 16 as seq_t */
    seq_t base = (seq_t)16;
    return seq_pow(base, e);
}
