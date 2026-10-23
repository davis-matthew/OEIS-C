/*
 * A167275  Row sums of triangle A167274 (a variant of Gould's sequence
 *          A001316).
 * https://oeis.org/A167275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 2^(popcount(n)+1) where *
 *            popcount(n) is the number of 1‑bits in the binary representation
 *            of n
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned popcount_u64(idx_t n)
{
    unsigned c = 0;
    while (n) {
        n &= n - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    unsigned exp = popcount_u64(n) + 1;   /* exponent of 2 */
    seq_t result = (seq_t)1;
    for (unsigned i = 0; i < exp; ++i)
        result <<= 1;                     /* multiply by 2 safely */
    return result;
}
