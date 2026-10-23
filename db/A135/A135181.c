/*
 * A135181  p^5 + p^3 - p^2. Exponents are the prime numbers in decreasing
 *          order and p is the n-th prime.
 * https://oeis.org/A135181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p^5 + p^3 - p^2, where p is the n‑th prime number
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 914, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The nth prime (1‑based).  get_prime returns a uint32_t. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute p^2, p^3, and p^5 using seq_t arithmetic. */
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p5 = p3 * p2;

    /* Return the desired expression. */
    return p5 + p3 - p2;
}
