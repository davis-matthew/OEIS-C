/*
 * A081256  Greatest prime factor of n^3 + 1.
 * https://oeis.org/A081256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = greatest prime factor of n^3 + 1
 * Range:     u8 n <= 17, u16 n <= 266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* compute n^3 + 1 safely in 64‑bit unsigned */
    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn * nn * nn + 1ULL;

    /* retrieve the largest prime divisor of m */
    uint64_t gpf = largest_prime_factor(m);

    return (seq_t)gpf;
}
