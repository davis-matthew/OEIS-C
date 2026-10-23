/*
 * A010180  Continued fraction for sqrt(115).
 * https://oeis.org/A010180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction expansion of sqrt(115). The period is 10 with
 *            first term 10 and repeating block {1,2,1,1,1,1,1,2,1,20}. So
 *            a(0) = 10, a(n) = block[(n-1) mod 10] for n > 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 10;
    static const seq_t block[] = {1, 2, 1, 1, 1, 1, 1, 2, 1, 20};
    int pos = (int)((n - 1) % (idx_t)10);
    return block[pos];
}
