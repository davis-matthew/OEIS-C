/*
 * A061286  Smallest integer for which the number of divisors is the n-th
 *          prime.
 * https://oeis.org/A061286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(prime(n)-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th prime (prime(1) = 2). */
    uint32_t p = get_prime(n);

    /* Compute 2^(p-1) by repeated left‑shift of 1. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < (idx_t)(p - 1); ++i)
        result <<= 1;

    return result;
}
