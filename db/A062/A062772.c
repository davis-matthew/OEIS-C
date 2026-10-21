/*
 * A062772  Smallest prime larger than square of n-th prime.
 * https://oeis.org/A062772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime > (p_n)^2, where p_n is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Obtain the n-th prime.  get_prime returns a uint32_t. */
    uint32_t p = get_prime(n);

    /* Compute the square of that prime in the output type. */
    seq_t sq = (seq_t)p * (seq_t)p;

    /* Find the smallest prime strictly greater than the square. */
    uint64_t nxt = next_prime((uint64_t)sq);

    /* Return the result cast to the sequence type. */
    return (seq_t)nxt;
}
