/*
 * A053388  a(n) = A053398(8, n).
 * https://oeis.org/A053388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of trailing 1-bits of (7 OR (n-1)), i.e.
 *            A053398(8,n) with (x-1)|(y-1), x=8
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 852 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t v = (n - 1) | (idx_t)7;
    seq_t pos = 0;
    while (v & 1) {
        v >>= 1;
        ++pos;
    }
    return pos;
}
