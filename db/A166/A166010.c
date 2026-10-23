/*
 * A166010  a(n) = prime(n)^2-4.
 * https://oeis.org/A166010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 - 4
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A166010 for any n >= 1 */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime.  The helper returns uint32_t,
       which is sufficient for all indices that typically occur
       in the tests (prime(10000) = 104729). */
    uint32_t p = get_prime(n);

    /* Compute prime^2 - 4 in the target integer type seq_t. */
    seq_t result = (seq_t)p * (seq_t)p - (seq_t)4;
    return result;
}
