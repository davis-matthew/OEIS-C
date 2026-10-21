/*
 * A072003  10's complement of final digit of n-th prime.
 * https://oeis.org/A072003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10 - (prime(n) mod 10)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime (OEIS a000040). */
    uint32_t p = get_prime(n);

    /* Compute 10's complement of its last decimal digit. */
    uint32_t rem = p % 10U;
    seq_t result = (seq_t)(10U - rem);

    return result;
}
