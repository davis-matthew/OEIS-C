/*
 * A093868  Smallest prime that differs from a multiple of n by unity.
 * https://oeis.org/A093868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min( smallest prime of the form n*k+1, smallest prime of
 *            the form n*k-1 )
 * Range:     u8 n <= 46, u16 n <= 2916, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements A093868: smallest prime that differs from a multiple
   of n by unity. Uses trial prime tests for small candidates. */

#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined below offset 1 */
    uint64_t un = (uint64_t)n;

    uint64_t p_plus  = 0;            /* smallest prime ≡ +1 (mod n) */
    uint64_t p_minus = 0;            /* smallest prime ≡ -1 (mod n) */

    /* search for n*k + 1 */
    for (uint64_t k = 1; ; ++k) {
        uint64_t candidate = un * k + 1;
        if (candidate < 2) continue;   /* skip values < 2 */
        if (is_prime_small(candidate)) {
            p_plus = candidate;
            break;
        }
    }

    /* search for n*k - 1 */
    for (uint64_t k = 1; ; ++k) {
        uint64_t candidate = un * k - 1;
        if (candidate < 2) continue;   /* skip values < 2 */
        if (is_prime_small(candidate)) {
            p_minus = candidate;
            break;
        }
    }

    /* return the smaller of the two primes found */
    return (seq_t)(p_plus < p_minus ? p_plus : p_minus);
}
