/*
 * A138840  Concatenation of initial and final digits of n-th prime.
 * https://oeis.org/A138840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = concatenation of the first and last digits of the n-th
 *            prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n must be >= 1. */
    if (n == 0) return 0;

    /* Fetch the n-th prime (1-indexed). */
    uint32_t p = get_prime(n);

    /* Determine first digit of p. */
    uint32_t first = p;
    while (first >= 10u)
        first /= 10u;

    /* Determine last digit of p. */
    uint32_t last = p % 10u;

    /* Concatenate the two digits and return as seq_t. */
    return (seq_t)first * 10 + last;
}
