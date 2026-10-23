/*
 * A171885  Representation of n in D. E. Knuth's second prefix-unambiguous,
 *          order-preserving binary string system.
 * https://oeis.org/A171885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2*4^⌊log₂n⌋ − 3*2^⌊log₂n⌋, with a(0)=0, a(1)=1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the two exceptional small values */
    if (n <= 1)
        return (seq_t)n;

    /* compute L = floor(log₂(n)) */
    unsigned L = 0;
    uint64_t t = n;
    while (t >>= 1)
        ++L;

    /* compute 2^L using seq_t multiplication */
    seq_t pow2L = 1;
    for (unsigned i = 0; i < L; ++i)
        pow2L *= 2;

    /* compute 4^L = (2^L)^2 */
    seq_t pow4L = pow2L * pow2L;

    /* term = 2*4^L − 3*2^L */
    seq_t term = (seq_t)2 * pow4L - (seq_t)3 * pow2L;

    /* a(n) = n + term */
    return (seq_t)n + term;
}
