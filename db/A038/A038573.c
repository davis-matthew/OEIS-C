/*
 * A038573  a(n) = 2^A000120(n) - 1.
 * https://oeis.org/A038573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^popcount(n) - 1 Implements the OEIS sequence A038573:
 *            a(n) = 2^A000120(n) - 1. popcount is the number of set bits in
 *            the binary representation of n
 * Range:     u8 n <= 510, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the population count of n using the GCC builtin */
    unsigned int pc = __builtin_popcountll((unsigned long long)n);

    /* 2^0 - 1 = 0, so we can return immediately for pc == 0 */
    if (pc == 0)
        return (seq_t)0;

    /* Compute 2^pc using the utility function and subtract 1 */
    seq_t res = pow2(pc);
    res -= (seq_t)1;
    return res;
}
