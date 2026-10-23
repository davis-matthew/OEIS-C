/*
 * A156660  Characteristic function of Sophie Germain primes.
 * https://oeis.org/A156660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a Sophie Germain prime (n and 2*n+1 both
 *            prime), else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* If n is not prime, a(n) = 0 immediately. */
    if (!is_prime(n))
        return 0;

    /* Compute 2*n+1 safely, detecting possible overflow. */
    if (n > (UINT64_MAX - 1) / 2)
        return 0;

    idx_t m = n * 2 + 1;
    return is_prime(m) ? (seq_t)1 : (seq_t)0;
}
