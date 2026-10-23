/*
 * A017439  a(n) = (11*n + 4)^3.
 * https://oeis.org/A017439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n + 4;
    seq_t res  = base * base * base;
    return res;
}
