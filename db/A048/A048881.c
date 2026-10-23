/*
 * A048881  a(n) = A000120(n+1) - 1 = wt(n+1) - 1.
 * https://oeis.org/A048881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000120(n+1) - 1 = popcount(n+1) - 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n + 1;                        /* n+1 (wraps naturally on overflow) */
    unsigned long long x = (unsigned long long)m;
    unsigned int c = 0;

    while (x) {
        x &= x - 1;        /* remove lowest set bit */
        c++;
    }

    return (seq_t)c - (seq_t)1;
}
