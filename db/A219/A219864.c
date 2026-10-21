/*
 * A219864  Number of ways to write n as p+q with p and 2pq+1 both prime.
 * https://oeis.org/A219864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (p, q) such that p + q = n, p is
 *            prime, and 2·p·q + 1 is prime. (q may be composite or 1.)
 * Range:     u8 n <= 4859, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;                 /* result to be returned */

    /* n < 3 cannot yield a valid pair: smallest prime is 2, q must be >= 1 */
    if (n < 3) return 0;

    for (idx_t p = 2; p < n; ++p) {  /* iterate over all possible primes p */
        if (!is_prime(p)) continue;       /* only prime p are considered */

        idx_t q = n - p;                  /* corresponding q */
        if (q == 0) continue;              /* q must be positive */

        /* Compute 2·p·q + 1 and test primality */
        idx_t prod = 2 * p * q + 1;
        if (is_prime(prod)) count++;
    }

    return count;
}
