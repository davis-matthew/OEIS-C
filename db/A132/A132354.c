/*
 * A132354  Integers m such that 7*m + 1 is a square.
 * https://oeis.org/A132354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = k*(7k + 2), a(2k+1) = 7k^2 + 12k + 5
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* k = floor(n/2) */
    seq_t ks = (seq_t)k;
    if ((n & 1ULL) == 0) {
        /* n is even: a(n) = k*(7k + 2) */
        return ks * (7 * ks + 2);
    } else {
        /* n is odd:  a(n) = 7k^2 + 12k + 5 */
        return 7 * ks * ks + 12 * ks + 5;
    }
}
