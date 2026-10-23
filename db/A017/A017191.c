/*
 * A017191  a(n) = (9*n + 2)^7.
 * https://oeis.org/A017191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 62, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)9 * n + 2;
    return base * base * base * base * base * base * base;
}
