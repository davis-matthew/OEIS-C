/*
 * A013797  a(n) = 12^(4*n + 3).
 * https://oeis.org/A013797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent 4*n + 3, computed using seq_t to avoid overflow */
    seq_t exp = (seq_t)n * 4 + 3;
    return big_pow((seq_t)12, exp);
}
