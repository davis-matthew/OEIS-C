/*
 * A179288  a(n) is the smallest prime number p that makes both 2p + prime(n)
 *          and p + 2*prime(n) prime numbers.
 * https://oeis.org/A179288
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest prime p such that 2p+prime(n) and p+2*prime(n) are both prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    /* For n < 2 the OEIS offset starts at 2; behavior is undefined below the offset. */
    if (n < 2)
        return 0;

    /* q is the n-th prime. */
    seq_t q = (seq_t)get_prime(n);

    /* Enumerate primes p until the required condition holds. */
    for (idx_t k = 1; ; ++k) {
        seq_t p = (seq_t)get_prime(k);

        seq_t val1 = 2 * p + q;     /* 2p + prime(n)   */
        seq_t val2 = p + 2 * q;     /* p + 2*prime(n)   */

        if (isPrimeGeneric(val1) && isPrimeGeneric(val2))
            return p;
    }
}
