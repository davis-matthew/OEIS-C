/*
 * A082500  a(n) = ceiling(n/2) if n is odd, or prime(n/2) otherwise.
 * https://oeis.org/A082500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = k, a(2k) = prime(k)
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* If n is odd, give (n+1)/2; otherwise return the (n/2)-th prime. */
    if ((n & 1) == 1)
        return (seq_t)((n + 1) / 2);
    else
        return (seq_t)get_prime(n / 2);
}
