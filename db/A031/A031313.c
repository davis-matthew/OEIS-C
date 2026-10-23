/*
 * A031313  Position of n-th 0 in A031312.
 * https://oeis.org/A031313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 97 + 150*floor((n-1)/5) + 3*((n-1)%5)
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    return (seq_t)97 + (seq_t)150 * (seq_t)k + (seq_t)3 * (seq_t)r;
}
