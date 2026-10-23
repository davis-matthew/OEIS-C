/*
 * A033441  Number of edges in 9-partite Turán graph of order n.
 * https://oeis.org/A033441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(8*i/9)
 * Range:     u8 n <= 24, u16 n <= 384, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute floor(8*i/9) using unsigned arithmetic */
        sum += (seq_t)((idx_t)8 * i / 9);
    }
    return sum;
}
