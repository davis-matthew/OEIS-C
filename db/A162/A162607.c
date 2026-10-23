/*
 * A162607  a(n) = n*(n^2 - 4*n + 5)/2.
 * https://oeis.org/A162607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - 4*n + 5)/2
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = N * N - (seq_t)4 * N + (seq_t)5;
    return N * t / (seq_t)2;
}
