/*
 * A152724  In binary, count of most frequent bit of n.
 * https://oeis.org/A152724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = max(popcount(n), bitlen(n)-popcount(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 1, a(3) = 2, ... as defined by OEIS A152724 */

#include <stdint.h>

/* count ones in the binary representation of an unsigned 64‑bit integer */
static inline unsigned popcount_u64(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/* length of the binary representation (position of highest 1 plus one) */
static inline unsigned bitlen_u64(uint64_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but return 0 for safety  */
        return 0;

    unsigned ones  = popcount_u64((uint64_t)n);
    unsigned bits  = bitlen_u64((uint64_t)n);
    unsigned zeros = bits - ones;

    return (seq_t)(ones > zeros ? ones : zeros);
}
