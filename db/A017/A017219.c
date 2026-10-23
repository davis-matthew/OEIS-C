/*
 * A017219  a(n) = (9*n + 4)^11.
 * https://oeis.org/A017219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 4)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 5, u128 n <= 353,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t base = (seq_t)9 * tn + (seq_t)4;
    return big_pow(base, (seq_t)11);
}
