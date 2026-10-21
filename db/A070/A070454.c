/*
 * A070454  a(n) = n^2 mod 32.
 * https://oeis.org/A070454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 32
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = n & 31ULL;          /* compute n modulo 32 using idx_t */
    seq_t x = (seq_t)m;              /* cast to seq_t for the arithmetic part */
    return (x * x) % (seq_t)32;      /* result fits in seq_t regardless of size */
}
