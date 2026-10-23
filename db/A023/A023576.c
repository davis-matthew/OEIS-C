/*
 * A023576  Greatest prime divisor of prime(n)+3.
 * https://oeis.org/A023576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime divisor of (prime(n) + 3)
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function obtains the n-th prime via get_prime(idx_t),
   adds 3, and returns the largest prime divisor of the result. */

#include "../utils/get_prime.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* n is 1-based as per OEIS convention.  The helper functions
       assume n >= 1; behaviour for n < 1 is undefined and not
       required by the sequence definition. */
    uint32_t p = get_prime(n);          /* nth prime, fits in 32 bits (for n <= 10^6) */
    uint64_t val = (uint64_t)p + 3ULL;  /* value to be factorised */
    uint64_t gpd = largest_prime_divisor(val); /* greatest prime divisor of val */
    return (seq_t)gpd;                  /* cast to the build‑specific seq_t type */
}
