/*
 * A017460  a(n) = (11*n + 5)^12.
 * https://oeis.org/A017460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 5)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 3, u128 n <= 147,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t base = n_seq * (seq_t)11 + (seq_t)5;
    return big_pow(base, (seq_t)12);
}
