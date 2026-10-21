/*
 * A208147  Sequence generated from A089080.
 * https://oeis.org/A208147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * (2 + v2(n-1)), a(1) = 1 where v2(x) is the
 *            exponent of 2 dividing x (the number of trailing zero bits in
 *            the binary representation of x)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned v2_uint64(uint64_t x)
{
    unsigned c = 0;
    /* x will never be 0 when this function is called (n >= 2) */
    while ((x & 1) == 0) {
        x >>= 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)1;      /* a(1) = 1 */

    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t k = i - 1;           /* (n-1) as a uint64_t */
        unsigned pow = 2 + v2_uint64(k);
        result *= (seq_t)pow;
    }
    return result;
}
