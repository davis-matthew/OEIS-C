/*
 * A014973  a(n) = n / gcd(n, (n-1)!).
 * https://oeis.org/A014973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / gcd(n, (n-1)!)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)          return 1;
    if (n == 4)          return 2;
    if (is_prime(n))     return (seq_t)n;
    return 1;
}
