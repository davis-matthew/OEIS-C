/*
 * A092754  a(1)=1, a(2n)=2a(n)+1, a(2n+1)=2a(n)+2.
 * https://oeis.org/A092754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor(log2(n)) + n - 1
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset is 1, n==0 is undefined; return 0 for safety */
    if (n == 0)
        return 0;

    /* Compute floor(log2(n)) without relying on external helpers. */
    unsigned k = 0;
    uint64_t t = (uint64_t)n >> 1;   /* shift once, start counting from 0 */
    while (t)
    {
        ++k;
        t >>= 1;
    }

    /* 2^k as a seq_t value.   k is at most 63 for uint64_t n. */
    seq_t pow2 = ((seq_t)1 << k);

    /* a(n) = 2^k + n - 1 */
    return pow2 + (seq_t)n - 1;
}
