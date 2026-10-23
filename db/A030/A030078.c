/*
 * A030078  Cubes of primes.
 * https://oeis.org/A030078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n))^3 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS: offset 1, so a(1) = 2^3 = 8 */
    if (n == 0)
        return 0;          /* undefined for n=0, return 0 */

    /* Retrieve the n-th prime (1‑based indexing). */
    idx_t p = get_prime(n);

    /* Compute the cube safely using 64‑bit arithmetic. */
    uint64_t tmp = (uint64_t)p;
    tmp = tmp * p * p;

    return (seq_t)tmp;
}
