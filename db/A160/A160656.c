/*
 * A160656  The odd prime numbers together with 0: p - (-1)^p - 1 where p =
 *          n-th prime.
 * https://oeis.org/A160656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = p - (-1)^p - 1 where p is the n-th prime * * For p == 2
 *            this yields 0, otherwise the result is simply p. * The
 *            implementation relies on the helper function get_prime(k) *
 *            which returns the k-th prime (1-based indexing)
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n); /* p is the n-th prime */
    return (p == 2) ? 0 : p;
}
