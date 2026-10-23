/*
 * A016962  a(n) = (6*n + 4)^6.
 * https://oeis.org/A016962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 270, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)4;
    return big_pow(base, (seq_t)6);
}
