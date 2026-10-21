/*
 * A096968  a(n) = (prime(n)*prime(n+2))^4.
 * https://oeis.org/A096968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)*prime(n+2))^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* retrieve the n-th and (n+2)-nd primes */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 2);

    /* product and raise to the fourth power */
    seq_t prod = p1 * p2;
    seq_t res  = prod * prod * prod * prod;

    return res;
}
