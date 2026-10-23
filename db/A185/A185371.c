/*
 * A185371  Product of two consecutive primes divided by the next prime and
 *          rounded down.
 * https://oeis.org/A185371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n) * prime(n+1) / prime(n+2))
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided utility get_prime to fetch the n-th prime. */

#include "../utils/get_prime.h"

/* External function that computes the A185371 sequence term. */
seq_t a(idx_t n)
{
    /* n is 1‑based; get_prime is assumed to return the n-th prime. */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 1);
    seq_t p3 = (seq_t)get_prime(n + 2);

    /* Integer division truncates toward zero, giving the floor for positive integers. */
    return (p1 * p2) / p3;
}
