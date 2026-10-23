/*
 * A135682  a(n)=n if n=1 or if n=prime. Otherwise, n=4 if n is even and n=7
 *          if n is odd.
 * https://oeis.org/A135682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n=1 or n is prime; otherwise 4 if n is even, 7 if n
 *            is odd
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)n;
    if (is_prime(n))
        return (seq_t)n;
    return ((n & 1) == 0) ? (seq_t)4 : (seq_t)7;
}
