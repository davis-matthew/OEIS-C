/*
 * A111701  Least integer obtained when n is divided by prime(1), then by
 *          prime(2), then by prime(3), ..., stopping as soon as one of the
 *          primes does not divide it. In particular, a(2n-1) = 2n-1.
 * https://oeis.org/A111701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n divided successively by 2,3,5,... until a prime does
 *            NOT divide the current value. Equivalent: a(n) = n divided by
 *            the largest primorial that divides n
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Convert index to the sequence type */
    seq_t curr = (seq_t)n;
    /* get_prime is 1‑based: get_prime(1)=2, get_prime(2)=3, … */
    unsigned long long prime_idx = 1;

    while (1) {
        seq_t p = get_prime(prime_idx);
        ++prime_idx;

        /* If the current value is not divisible by this prime,
           we stop and return the remaining quotient. */
        if (curr % p != 0)
            break;

        /* Divide once by the prime and continue with the next one. */
        curr /= p;

        /* If the result has become 1 we cannot divide further. */
        if (curr == 1)
            break;
    }

    return curr;
}
