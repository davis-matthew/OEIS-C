/*
 * A138751  a(n) = nextprime( p(n)/2 if p(n)=2 (mod 3), 2p(n) else ) =
 *          A007918( A138750( A000040( n ))).
 * https://oeis.org/A138751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nextprime( p(n)/2 if p(n)=2 (mod 3), 2p(n) else ) where
 *            p(n) is the n‑th prime. The function uses get_prime to fetch
 *            p(n) and next_prime (from the utils library) to obtain the next
 *            prime strictly larger than the specified bound. The code works
 *            for any seq_t implementation (uint8, 16, 32, 6...
 * Range:     u8 n <= 30, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* get the n-th prime as a 32‑bit unsigned integer */
    uint32_t p = get_prime(n);

    /* compute the bound for the next prime */
    uint64_t lower;
    if ((p % 3) == 2)            /* p ≡ 2 (mod 3) → use p/2 (floor)   */
        lower = (uint64_t)p / 2;
    else                          /* otherwise use 2p                */
        lower = (uint64_t)p * 2;

    /* return the next prime strictly greater than that bound */
    return (seq_t)next_prime(lower);
}
