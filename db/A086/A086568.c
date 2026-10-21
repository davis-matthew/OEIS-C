/*
 * A086568  Smallest number having as many distinct prime factors as n has
 *          prime factors, when counted with multiplicity.
 * https://oeis.org/A086568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primorial of bigomega(n) = product of the first
 *            bigomega(n) primes
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 1023, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   bigomega, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset starts at 1, so n==0 is not used but handled gracefully */

    /* total number of prime factors of n (with multiplicity) */
    idx_t w = (idx_t)bigomega(n);

    seq_t result = 1;
    for (idx_t i = 1; i <= w; ++i)
        result *= (seq_t)get_prime(i);

    return result;
}
