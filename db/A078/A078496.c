/*
 * A078496  Smallest prime p such that p>n and 2*n-p is also prime.
 * https://oeis.org/A078496
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{p : p>n and p and 2*n-p are primes}
 * Range:     u8 n <= 222, u16 n <= 9430, u32 n <= 9430, u64 n <= 9430,
 *            u128 n <= 9430, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(4) is the first defined term; for n < 4 the value is undefined. */
    if (n < 4)
        return 0;

    idx_t p     = n + 1;          /* start searching at the first prime after n */
    idx_t limit = 2 * n - 2;      /* p must satisfy 2*n - p ≥ 2 */

    for (; p <= limit; ++p) {
        if (is_prime(p) && is_prime(2 * n - p))
            return (seq_t)p;
    }
    /* If we reach this point no such prime was found; this
       should not happen for n ≥ 4 according to the definition. */
    return 0;
}
