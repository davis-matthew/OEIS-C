/*
 * A059152  A hierarchical sequence (W'2{2}c - see A059126).
 * https://oeis.org/A059152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*4^A007814(n) where A007814(n) is 0 for even n, and for
 *            odd n: A007814(n) = 1 + v₂((n+1)/2), v₂(x) = exponent of 2 in x
 * Range:     u8 n <= 6, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   tz_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/tz_uint64.h"

seq_t a(idx_t n)
{
    /* For even indices the value is always 4. */
    if ((n & 1ULL) == 0)
        return (seq_t)4;

    /* n is odd */
    idx_t m = (n + 1) >> 1;          /* (n+1)/2 */
    int tz = tz_uint64(m);           /* v₂(m) */
    int exp = 4 + (2 * tz);          /* exponent of 2 in a(n) */

    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= (seq_t)2;

    return result;
}
