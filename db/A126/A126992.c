/*
 * A126992  a(n) is the largest prime smaller than sqrt(prime(n)*prime(n+6)).
 * https://oeis.org/A126992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime smaller than sqrt(prime(n) * prime(n+6))
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 2980, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note: use the provided helpers get_prime, floor_sqrt and is_prime. */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Retrieve the n-th and (n+6)-th primes (1‑based, as used by get_prime). */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 6);

    /* Compute the product of the two primes. */
    seq_t product = p1 * p2;

    /* Compute the integer floor of the square root of the product. */
    seq_t sq = floor_sqrt(product);

    /* If the product is a perfect square, do not use the square root itself,
       because the definition requires a prime strictly smaller than the sqrt. */
    if (sq * sq == product) {
        sq -= 1;
    }

    /* Search downward from sq for the largest prime ≤ sq. */
    seq_t candidate = sq;
    while (!is_prime((idx_t)candidate)) {
        candidate -= 1;
    }

    return candidate;
}
