/*
 * A195030  a(n) = (n-2)*(14*n-39) for n > 2, otherwise a(n) = n.
 * https://oeis.org/A195030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n <= 2, a(n) = n; for n > 2, a(n) = (n-2)*(14*n-39)
 * Range:     u8 n <= 6, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n <= 2)
        return nn;
    seq_t t1 = nn - (seq_t)2;
    seq_t t2 = (seq_t)14 * nn - (seq_t)39;
    return t1 * t2;
}
