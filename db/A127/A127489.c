/*
 * A127489  a(n) is the coefficient of the linear term in the polynomial
 *          (x-prime(n))*(x-prime(n+1))*(x-prime(n+2))*(x-prime(n+3))*(x-prime(n+4)).
 * https://oeis.org/A127489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = arithmetic derivative of the product of five consecutive
 *            primes. * That is, a(n) = Σ_{i=0..4} (P_n P_{n+1} … P_{n+4} /
 *            P_{n+i}), * where P_k is the k-th prime
 * Range:     u8 none, u16 n <= 2, u32 n <= 21, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the five consecutive primes. */
    seq_t primes[5];
    for (idx_t i = 0; i < 5; ++i)
        primes[i] = (seq_t) get_prime(n + i);

    /* Compute the product of the five primes. */
    seq_t prod = 1;
    for (idx_t i = 0; i < 5; ++i)
        prod *= primes[i];

    /* Compute the sum of product divided by each prime. */
    seq_t sum = 0;
    for (idx_t i = 0; i < 5; ++i)
        sum += prod / primes[i];

    return sum;
}
