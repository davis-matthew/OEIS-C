/*
 * A160727  a(n) = A161415(n+1)/4.
 * https://oeis.org/A160727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * a(n) = 3^popcount(n) * - (n+1)/2 if n+1 is a power of 2 (i.e.
 *            n = 2^k-1) * otherwise
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence (offset 1) */
    idx_t x = n + 1;                /* n+1, used for power-of-two test */

    /* popcount of n (number of 1 bits) */
    int pc = __builtin_popcountll(n);

    /* 3^popcount(n) */
    seq_t val = 1;
    for (int i = 0; i < pc; ++i)
        val *= (seq_t)3;

    /* Adjustment for n = 2^k - 1 */
    if ((x & (x - 1)) == 0) {      /* x is a power of two */
        seq_t sub = (seq_t)(x >> 1); /* (n+1)/2 */
        val -= sub;
    }

    return val;
}
