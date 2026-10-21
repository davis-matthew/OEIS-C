/*
 * A050603  A001511 with every term repeated.
 * https://oeis.org/A050603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + v2(floor(n/2)+1) where v2(m) is the exponent of 2 in
 *            m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n >> 1;           /* floor(n/2) */
    idx_t x = m + 1;            /* x >= 1 */
    unsigned int cnt = 0;
    while ((x & 1) == 0) {      /* count trailing zeros of x */
        cnt++;
        x >>= 1;
    }
    return (seq_t)(cnt + 1);
}
