/*
 * A216522  Integers of the form 2*x + 3*y with nonnegative x and y, with
 *          repetitions.
 * https://oeis.org/A216522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(12*n + 4)) - 2
 * Range:     u8 n <= 4, u16 n <= 500, u32 n <= 8240, u64 n <= 8240,
 *            u128 n <= 8240, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * (seq_t)12 + 4;
    seq_t s = isqrt_seq(val);
    return s - 2;
}
