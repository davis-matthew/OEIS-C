/*
 * A023590  Greatest prime divisor of 2*prime(n)+1.
 * https://oeis.org/A023590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime divisor of (2*prime(n)+1)
 * Range:     u8 n <= 31, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A023590.
   It uses helper functions from the utils library:
     - get_prime(idx_t n)          : returns the nth prime (1‑based).
     - largest_prime_divisor(uint64_t m)
        : returns the largest prime divisor of m.
*/
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* If n is less than 1 the result is undefined; return 0. */
    if (n < 1)
        return 0;

    /* Fetch the nth prime. */
    uint64_t p = get_prime(n);

    /* Compute m = 2*p + 1 (fits in 64 bits for realistic n). */
    uint64_t m = 2ull * p + 1ull;

    /* Find its greatest prime divisor. */
    uint64_t g = largest_prime_divisor(m);

    return (seq_t)g;
}
