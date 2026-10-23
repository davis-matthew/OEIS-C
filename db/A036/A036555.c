/*
 * A036555  Hamming weight of 3n: number of 1's in binary expansion of 3n.
 * https://oeis.org/A036555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = popcount(3*n) – the number of 1-bits in the binary
 *            expansion of 3*n
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute x = 3*n using seq_t to avoid overflow */
    seq_t x = (seq_t)n * 3;
    seq_t count = 0;

    /* Count set bits using the classic x &= x-1 trick.
       This works for any unsigned integer type, including arbitrary‑precision. */
    while (x)
    {
        count++;
        x &= x - 1;
    }
    return count;
}
