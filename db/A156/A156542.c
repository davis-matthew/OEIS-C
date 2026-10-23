/*
 * A156542  Number of distinct Sophie Germain prime factors of n.
 * https://oeis.org/A156542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} A101264(p), where p is a prime. A101264(p) = 1
 *            if p is a Sophie Germain prime (p and 2p+1 are primes), 0
 *            otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t temp = n;
    seq_t count = 0;

    /* Handle factor 2 */
    if ((temp & 1ULL) == 0) {
        /* 2 is a Sophie Germain prime because 2*2+1 = 5 is prime */
        if (is_prime(5))
            count++;
        while ((temp & 1ULL) == 0)
            temp >>= 1;
    }

    /* Trial division for odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            /* p is a prime factor; count it if 2p+1 is also prime.
               Avoid overflow in 2p+1. */
            if (p <= (UINT64_MAX - 1ULL) / 2ULL) {
                if (is_prime((idx_t)(2 * p + 1)))
                    count++;
            }
            /* Remove all powers of p */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* Remaining prime factor > 1 */
    if (temp > 1) {
        idx_t p = temp; /* temp is prime */
        if (p <= (UINT64_MAX - 1ULL) / 2ULL) {
            if (is_prime((idx_t)(2 * p + 1)))
                count++;
        }
    }

    return count;
}
