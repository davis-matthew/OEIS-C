/*
 * A205382  s(k)-s(j), where (s(k),s(j)) is the least such pair for which n
 *          divides their difference, and s(j)=(2j-1)^2.
 * https://oeis.org/A205382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, 8)
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute gcd(n, 8) using the fact that 8 = 2³ */
    uint64_t g;
    if ((n & 7ULL) == 0ULL)          g = 8;
    else if ((n & 3ULL) == 0ULL)     g = 4;
    else if ((n & 1ULL) == 0ULL)     g = 2;
    else                            g = 1;

    /* lcm(n, 8) = n * 8 / gcd(n, 8) */
    seq_t mul = (seq_t)(8 / g);
    return (seq_t)n * mul;
}
