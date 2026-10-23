/*
 * A035026  Number of times that i and 2n-i are both prime, for i = 1, ...,
 *          2n-1.
 * https://oeis.org/A035026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{(p,q) : p and q are primes, p+q = 2n, 1 <= p <= 2n-1}|
 * Range:     u8 n <= 1364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 2n is the target sum.  We generate all primes up to 2n and
       count those whose complement is also prime. */
    idx_t limit = 2 * n;                 /* sum of the two primes            */
    uint64_t *primes = NULL;             /* array of primes up to limit      */
    size_t prime_count = 0;

    /* Build the list of primes up to 'limit' (inclusive). */
    build_primes_upto(limit, &primes, &prime_count);

    seq_t count = 0;                     /* result to return                 */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p >= limit)                 /* only up to 2n-1                  */
            break;
        uint64_t q = limit - p;          /* 2n - p                          */
        if (q >= 2 && is_prime((idx_t)q))
            ++count;                     /* both p and q are prime          */
    }

    free(primes);                       /* deallocate memory used for primes */

    return count;
}
