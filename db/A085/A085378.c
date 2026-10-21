/*
 * A085378  Difference between primes p and the largest prime divisor of p-1.
 * https://oeis.org/A085378
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p_n - lpd(p_n - 1) where p_n is the n‑th prime (1‑based)
 *            and lpd(m) is the largest prime divisor of m. The function
 *            returns 0 for n < 3 (offset handling is irrelevant)
 * Range:     u8 n <= 57, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                     /* offset behavior is undefined */

    /* Retrieve the n-th prime (1‑based). */
    uint32_t p32 = get_prime(n);
    uint64_t p   = (uint64_t)p32;     /* promote for arithmetic safety */

    /* Compute the largest prime divisor of p-1. */
    uint64_t lpf = largest_prime_divisor(p - 1ULL);

    /* Return the required difference. */
    return (seq_t)(p - lpf);
}
