/*
 * A153875  3 times 13-gonal (or tridecagonal) numbers: a(n) = 3*n*(11*n -
 *          9)/2.
 * https://oeis.org/A153875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(11*n-9)/2
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = ((seq_t)3 * nn * ((seq_t)11 * nn - (seq_t)9)) / (seq_t)2;
    return val;
}
