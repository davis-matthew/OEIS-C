/*
 * A072565  a(n) = prime(n+1)*prime(n+2)+1 mod prime(n), where prime(n) is the
 *          n-th prime.
 * https://oeis.org/A072565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)*prime(n+2)+1 mod prime(n)
 * Range:     u8 n <= 65, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset: n >= 1 */
    if (n == 0)
        return 0;   /* undefined for offset, but return 0 */

    /* Retrieve the required primes.  get_prime returns an
       unsigned 32‑bit integer (uint32_t). */
    uint32_t pn   = get_prime(n);
    uint32_t pn1  = get_prime(n + 1);
    uint32_t pn2  = get_prime(n + 2);

    /* Compute (pn1 * pn2 + 1) modulo pn.  Use a 64‑bit intermediate
       to avoid overflow on small seq_t (e.g., 8, 16, 32 bits). */
    uint64_t mod    = (uint64_t)pn;
    uint64_t t1     = (uint64_t)pn1 % mod;
    uint64_t t2     = (uint64_t)pn2 % mod;
    uint64_t prod   = t1 * t2 + 1;
    uint64_t result = prod % mod;

    return (seq_t)result;
}
