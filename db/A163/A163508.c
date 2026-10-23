/*
 * A163508  The sum of the prime factors (with repetition) of the sum of 2
 *          successive primes.
 * https://oeis.org/A163508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sopfr(p_n + p_(n+1))
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sopfr(m) is the sum of prime factors of m counted with multiplicity. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Assumes n >= 1. */
    uint64_t p1 = (uint64_t)get_prime(n);
    uint64_t p2 = (uint64_t)get_prime(n + 1);
    uint64_t sum_of_primes = p1 + p2;

    seq_t sum = 0;
    while (sum_of_primes > 1) {
        uint64_t sp = smallest_prime_factor(sum_of_primes);
        sum += (seq_t)sp;
        sum_of_primes /= sp;
    }
    return sum;
}
