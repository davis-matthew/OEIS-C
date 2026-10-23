/*
 * A032608  Concatenation of n and n + 3.
 * https://oeis.org/A032608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of n and n+3
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    return concat_parts(n, n + 3);
}
