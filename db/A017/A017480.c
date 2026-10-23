/*
 * A017480  a(n) = (11*n + 7)^8.
 * https://oeis.org/A017480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 22, u128 n <= 5957,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * n + 7U;
    return seq_pow(base, 8U);
}
