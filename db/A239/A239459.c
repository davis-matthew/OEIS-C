/*
 * A239459  Concatenation of n^3 and n.
 * https://oeis.org/A239459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_parts(n^3, n)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;
    return concat_parts((idx_t)n3, n);
}
