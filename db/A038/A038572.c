/*
 * A038572  a(n) = n rotated one binary place to the right.
 * https://oeis.org/A038572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn, nice
 * Formula:   a(n) = 0 for n = 0; otherwise a(n) = (n >> 1) | ((n & 1) <<
 *            (len-1)), where len = number of bits needed to represent n
 * Range:     u8 n <= 255, u16 n <= 9529, u32 n <= 9529, u64 n <= 9529,
 *            u128 n <= 9529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute bit length of n.  n is uint64_t. */
    unsigned len = 64 - __builtin_clzll((unsigned long long)n);
    seq_t lsb = (seq_t)(n & 1);
    return ( (seq_t)n >> 1 ) | ( lsb << (len - 1) );
}
