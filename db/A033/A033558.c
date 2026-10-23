/*
 * A033558  a(n) = 2n - {smallest prime > n}.
 * https://oeis.org/A033558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - smallest prime > n
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Find the smallest prime strictly greater than n. */
    uint64_t np = next_prime((uint64_t)n);

    /* Compute 2*n and subtract the next prime. */
    seq_t two_n = (seq_t)n * 2;
    return two_n - (seq_t)np;
}
