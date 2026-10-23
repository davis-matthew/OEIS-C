/*
 * A155484  a(n) = 3169^(2n + 1).
 * https://oeis.org/A155484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3169^(2n + 1)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t exp_idx = n * (idx_t)2 + (idx_t)1;
    seq_t exp = (seq_t)exp_idx;
    return big_pow((seq_t)3169, exp);
}
