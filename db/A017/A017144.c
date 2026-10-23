/*
 * A017144  a(n) = (8*n + 6)^8.
 * https://oeis.org/A017144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 6)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)8 + (seq_t)6;
    return pow_seq(m, 8);
}
