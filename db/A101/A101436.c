/*
 * A101436  Number of exponents in prime factorization of n which are primes.
 * https://oeis.org/A101436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime exponents in the prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p^e) = 1 if e is prime, otherwise 0; a(n) = sum of a(p^e) over all prime factors p of n */

#include <stdbool.h>
#include "../utils/is_prime_small.h"  // provided helper

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n <= 1) return 0;

    idx_t temp = n;
    idx_t p = 2;

    while (p <= temp / p) {
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        if (exp > 0 && is_prime_small((int)exp)) {
            count++;
        }
        if (p == 2) {
            p = 3;
        } else {
            p += 2;  // skip even numbers
        }
    }

    /* if temp > 1, it is a prime factor with exponent 1 (not prime), so ignore */

    return count;
}
