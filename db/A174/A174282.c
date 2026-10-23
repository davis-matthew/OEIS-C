/*
 * A174282  a(n) = 3^n mod M(n) where M(n) = A014963(n) is the exponential of
 *          the Mangoldt function.
 * https://oeis.org/A174282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = p^k for a prime p ≠ 3 (k > 0); otherwise a(n) =
 *            0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return 1 or 0 according to the definition of the sequence A174282. */
seq_t a(idx_t n)
{
    /* The definition applies only for n ≥ 1.  For n ≤ 1 the answer is 0. */
    if (n <= 1)
        return (seq_t)0;

    /* Work in native 64‑bit arithmetic.  The value of n fits in uint64_t. */
    uint64_t nn = (uint64_t)n;
    uint64_t p  = 0;          /* smallest prime factor of nn */

    /* Find the smallest prime factor by trial division. */
    if (nn % 2 == 0)
        p = 2;
    else
    {
        for (uint64_t i = 3; i <= nn / i; i += 2)
        {
            if (nn % i == 0)
            {
                p = i;
                break;
            }
        }
    }
    if (p == 0)
        /* nn no factor found – nn is prime. */
        p = nn;

    /* If the prime factor is 3, the value is 0. */
    if (p == 3)
        return (seq_t)0;

    /* Determine whether nn is a pure power of p. */
    while (nn % p == 0)
        nn /= p;

    /* If we have reduced to 1, nn = p^k for some k > 0. */
    return (nn == 1) ? (seq_t)1 : (seq_t)0;
}
