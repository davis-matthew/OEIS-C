/*
 * A110269  n mod 2 + n mod 3.
 * https://oeis.org/A110269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 2 + n mod 3
 * Range:     u8 n <= 274, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t r2 = x & 1;        /* x mod 2 */
    seq_t r3 = x % 3;        /* x mod 3 */
    return r2 + r3;
}
