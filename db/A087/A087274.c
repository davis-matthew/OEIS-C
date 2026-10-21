/*
 * A087274  Prime index of the largest prime factor of 3*prime(n)+1.
 * https://oeis.org/A087274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(maxPrimeFactor(3*prime(n)+1))
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Offset 1: for n < 1 return 0 (undefined behavior in OEIS) */
    if (n < 1) return 0;

    /* Step 1: find the n-th prime (prime(n)) */
    uint32_t p = get_prime(n); /* fits 32 bits for N up to ~10^6 */

    /* Step 2: compute 3*prime(n)+1 */
    uint64_t val = 3ULL * (uint64_t)p + 1ULL;

    /* Step 3: largest prime divisor of the number */
    uint64_t lpd = largest_prime_divisor(val);

    /* Step 4: prime index pi(lpd) */
    seq_t res = compute_pi(lpd); /* returns seq_t */

    return res;
}
