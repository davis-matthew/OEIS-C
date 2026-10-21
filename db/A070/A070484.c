/*
 * A070484  a(n) = n^3 mod 22.
 * https://oeis.org/A070484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 22
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n % 22 fits in uint64_t */
    uint64_t m = n % 22;
    uint64_t res = (m * m) % 22;
    res = (res * m) % 22;
    return (seq_t)res;
}
