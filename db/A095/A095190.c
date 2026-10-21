/*
 * A095190  Doubled Thue-Morse sequence: a(2n) = A010060(n), a(2n+1) =
 *          A010060(n).
 * https://oeis.org/A095190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = parity of popcount(floor(n/2)) which equals popcount(n >>
 *            1) mod 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* shift right by one (floor division by 2) */
    uint64_t m = n >> 1;
    seq_t parity = 0;
    /* Count set bits modulo 2 using pop‑count via bit trick */
    while (m) {
        parity ^= 1;   /* toggle parity for each set bit */
        m &= m - 1;    /* remove lowest set bit */
    }
    return parity;    /* 0 or 1 fits in any seq_t type */
}
