/*
 * A225587  a(n) is the smallest prime of the form prime(n)*q + prime(n) + q,
 *          where q is an odd prime.
 * https://oeis.org/A225587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime of the form prime(n)*q + prime(n) + q,
 *            where q is an odd prime (search over odd primes)
 * Range:     u8 n <= 13, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, isPrimeGeneric, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */
    /* p = the n-th prime, 1‑based */
    seq_t p = (seq_t)get_prime(n);
    seq_t q = 3;  /* first odd prime */
    while (true) {
        seq_t val = p * q + p + q;  /* value to test */
        if (isPrimeGeneric(val))
            return val;
        /* move to the next odd prime */
        q = (seq_t)next_prime((uint64_t)q + 1);
    }
}
