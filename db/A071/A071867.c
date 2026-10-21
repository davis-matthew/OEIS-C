/*
 * A071867  Maximum element in the continued fraction for prime(n+1)/prime(n).
 * https://oeis.org/A071867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = maximum element in the continued‑fraction expansion of
 *            prime(n+1)/prime(n)
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so indices < 1 are undefined. */
    if (n < 1)
        return 0;

    /* Fetch the consecutive primes.  get_prime returns a uint32_t, cast
       to 64‐bit to safely perform the division and remainder. */
    uint64_t p1 = (uint64_t)get_prime(n);     /* prime(n)   */
    uint64_t p2 = (uint64_t)get_prime(n + 1); /* prime(n+1) */

    seq_t max_term = 0;

    /* Euclidean algorithm – generates the continued‑fraction terms of p2/p1.
       We keep the maximum term seen. */
    while (p1 != 0) {
        seq_t q = (seq_t)(p2 / p1);
        if (q > max_term)
            max_term = q;
        uint64_t r = p2 % p1;
        p2 = p1;
        p1 = r;
    }

    return max_term;
}
