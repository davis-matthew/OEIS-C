/*
 * A213671  The odd part of n^2 - n + 2.
 * https://oeis.org/A213671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   odd part of n^2 - n + 2
 * Range:     u8 n <= 16, u16 n <= 344, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * (seq_t)n - (seq_t)n + 2;
    return odd_part(x);
}
