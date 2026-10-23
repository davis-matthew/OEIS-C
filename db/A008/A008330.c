/*
 * A008330  phi(p-1), as p runs through the primes.
 * https://oeis.org/A008330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = phi(prime(n) - 1)
 * Range:     u8 n <= 54, u16 n <= 7333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities provided by the build environment. */
#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/phi.h"         /* seq_t phi(idx_t m); */

seq_t a(idx_t n)
{
    if (n < 1) {
        /* For indices below the offset, the behavior is not defined.
           Return 0 as a harmless sentinel. */
        return (seq_t)0;
    }

    /* Obtain the n-th prime. The function get_prime is 1‑based. */
    uint32_t p = get_prime(n);

    /* Compute the Euler totient of p-1.  The argument to phi is idx_t,
       but p-1 fits comfortably into uint64_t so the cast is safe. */
    return phi((idx_t)(p - 1ULL));
}
