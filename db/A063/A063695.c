/*
 * A063695  Remove even-positioned bits from the binary expansion of n.
 * https://oeis.org/A063695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n with all bits at even positions cleared. In other
 *            words, a(n) = ∑_{k≥0} 2^(2k+1) * ((n >> (2k+1)) & 1)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: indexes start at 0 (a(0) = 0). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the number of bits needed to represent n (excluding zero). */
    unsigned len = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++len;
    }

    seq_t result = 0;
    /* For each odd bit position < len, copy the bit from n to the same position in result. */
    for (unsigned i = 1; i < len; i += 2) {
        if ((n >> i) & 1ULL) {
            result |= ((seq_t)1 << i);
        }
    }
    return result;
}
