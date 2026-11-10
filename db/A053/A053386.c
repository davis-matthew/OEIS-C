/*
 * A053386  A053398(6, n).
 * https://oeis.org/A053386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053398(6,n) = position of the least significant 0 bit of
 *            (5 | (n-1)), i.e. the number of trailing 1 bits (Kopper's Nim
 *            value)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 907 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t v = (idx_t)5 | (n - 1);
    unsigned pos = 0;
    while (v & 1) {
        v >>= 1;
        ++pos;
    }
    return (seq_t)pos;
}
