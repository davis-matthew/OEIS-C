/*
 * A227361  If n is even, then a(n) = n + bitsum(n), else a(n) = n -
 *          bitsum(n), where bitsum(n) is the count of binary 1's in n,
 *          A000120.
 * https://oeis.org/A227361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = { n + bitsum(n) if n even * n - bitsum(n) if n odd } *
 *            where bitsum(n) is the number of 1-bits in the binary
 *            representation of n
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Count the number of set bits in an idx_t value. */
static inline size_t popcount_u64(idx_t x)
{
    size_t c = 0;
    while (x) {
        x &= x - 1;   /* clear the lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    size_t bits = popcount_u64(n);
    if ((n & 1ULL) == 0) {
        return (seq_t)n + (seq_t)bits;   /* n is even */
    } else {
        return (seq_t)n - (seq_t)bits;   /* n is odd  */
    }
}
