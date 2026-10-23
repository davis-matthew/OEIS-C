/*
 * A037834  a(n) = Sum_{i=1..m} |d(i) - d(i-1)|, where Sum_{i=0..m} d(i)*2^i
 *          is the base-2 representation of n.
 * https://oeis.org/A037834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = popcount(n ^ (n >> 1)) - 1 * This counts the number of
 *            bit transitions in the binary representation * of n, which
 *            equals A037834
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is an idx_t (uint64_t). Compute the bit transition count
     * using the efficient popcount instruction. */
    unsigned long long bits = n ^ (n >> 1);      /* differences between consecutive bits
                                                   + an extra leading 1 for the MSB */
    unsigned int transitions = __builtin_popcountll(bits) - 1;
    return (seq_t)transitions;
}
