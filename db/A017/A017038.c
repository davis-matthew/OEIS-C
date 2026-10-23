/*
 * A017038  a(n) = (7*n + 4)^10.
 * https://oeis.org/A017038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 11, u128 n <= 1018,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * (seq_t)n + (seq_t)4;
    return seq_pow(base, (idx_t)10);
}
