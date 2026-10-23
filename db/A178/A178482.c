/*
 * A178482  Phi-antipalindromic numbers.
 * https://oeis.org/A178482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1. For n ≥ 2, let k = ⌊log2(n)⌋. Then a(n) = a(2^k) + a(n
 *            − 2^k). This is equivalent to summing Lucas numbers over the
 *            bits of n: for each bit position k ≥ 1 that is set in n, add
 *            Lucas(2k); if the least significant bit is set, add 1 (since
 *            a(1)=1)
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lucas_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/lucas_index.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for 0, but safe fallback */

    seq_t result = (seq_t)0;

    /* contribution of the least significant bit */
    if (n & (idx_t)1) result += (seq_t)1;

    /* iterate over remaining bits, starting at position 1 */
    idx_t m = n >> (idx_t)1;   /* shift off the LSB */
    uint64_t k = 1;            /* bit position (k>=1) */

    while (m != (idx_t)0)
    {
        if (m & (idx_t)1)
            result += lucas_index( (idx_t)(2ULL * k) );   /* Lucas(2k) */

        m >>= (idx_t)1;
        ++k;
    }

    return result;
}
