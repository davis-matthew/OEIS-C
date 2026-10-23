/*
 * A156659  Characteristic function of safe primes.
 * https://oeis.org/A156659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime and (n-1)/2 is prime, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* A safe prime is defined by n being prime and (n-1)/2 also being prime.
       The smallest safe prime is 5, so for n < 5 the answer is 0.
       For safety with unsigned arithmetic we guard against underflow
       before computing (n-1)/2. */
    if (n < 5) {
        return 0;
    }

    if (is_prime(n) && is_prime((n - 1) / 2)) {
        return (seq_t)1;
    }
    return (seq_t)0;
}
