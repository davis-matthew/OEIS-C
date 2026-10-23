/*
 * A106524  Interleave A038573(n+1) and 2*A038573(n+1).
 * https://oeis.org/A106524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Interleave A038573(n+1) and 2*A038573(n+1) where
 *            A038573(m) = 2^{ popcount(m) } – 1 popcount(m) = number of
 *            1‑bits in the binary representation of m
 * Range:     u8 n <= 508, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = n/2 + 1 for even n, or (n-1)/2 + 1 for odd n   */
    idx_t m = (n >> 1) + 1ULL;                 /* floor(n/2)+1 */

    /* popcount of m (assumes idx_t is an unsigned 64‑bit type) */
    unsigned int pop = __builtin_popcountll((unsigned long long)m);

    /* compute 2^pop as a seq_t by repeated doubling */
    seq_t val = 1;
    for (unsigned int i = 0; i < pop; ++i)
        val <<= 1;          /* val = 2^pop */

    val -= 1;               /* A038573(m) = 2^pop - 1 */

    if (n & 1)              /* odd n -> double the value */
        val <<= 1;          /* val = 2 * (2^pop - 1) */

    return val;
}
