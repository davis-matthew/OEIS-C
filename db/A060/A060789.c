/*
 * A060789  a(n) = n / (gcd(n,2) * gcd(n,3)).
 * https://oeis.org/A060789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n / (gcd(n,2)*gcd(n,3))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t g2 = (n & 1ULL) ? 1ULL : 2ULL;
    idx_t g3 = (n % 3ULL) ? 1ULL : 3ULL;
    idx_t denom = g2 * g3;     /* denom is 1, 2, 3, or 6 */
    return (seq_t)n / (seq_t)denom;
}
