/*
 * A023593  Exponent of least prime factor of 2*prime(n)+1.
 * https://oeis.org/A023593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of least prime factor of 2*prime(n)+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/_smallest_prime_factor.h"

/* Compute a(n) for n ≥ 1.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* safe for invalid indices */

    /* 1. Retrieve the n‑th prime. */
    uint64_t p = (uint64_t)get_prime(n);

    /* 2. Compute m = 2*p + 1. */
    uint64_t m = 2 * p + 1;

    /* 3. Find the least prime factor of m. */
    uint64_t lpf = _smallest_prime_factor(m);

    /* 4. Count the exponent of lpf in m. */
    seq_t exp = 0;
    while (m % lpf == 0) {
        m /= lpf;
        ++exp;
    }

    return exp;
}
