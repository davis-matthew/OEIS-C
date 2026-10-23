/*
 * A113047  a(n) = C(3n,n)/(2n+1) mod 3.
 * https://oeis.org/A113047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if 2*n+1 is a power of 3, else 0 This is the
 *            characteristic function of ternary repunits (n = (3^j-1)/2 for
 *            some j ≥ 0)
 * Range:     u8 n <= 9779, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For all practical n (≤ 10000) 2*n+1 fits in 64 bits. */
    unsigned long long target = (unsigned long long)2 * n + 1ull;
    unsigned long long power  = 1ull;
    while (power < target)
        power *= 3ull;
    return (power == target) ? 1 : 0;
}
