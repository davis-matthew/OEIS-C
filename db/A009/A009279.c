/*
 * A009279  a(n) = lcm(d(n), phi(n)).
 * https://oeis.org/A009279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(d(n), φ(n))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   _my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes d(n) (number of divisors) and φ(n) (Euler totient) by trial division. */
#include <stdint.h>
#include "../utils/_my_gcd.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset starts at 1; undefined for 0 */
    /* local copies using 64‑bit integers for trial division */
    uint64_t rem = (uint64_t)n;
    seq_t phi = (seq_t)n;          /* start with φ(n) = n */
    seq_t d   = (seq_t)1;          /* start with d(n) = 1 */

    for (uint64_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            int exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }
            /* update totient: φ *= (p-1)/p */
            phi   = phi / (seq_t)p * ((seq_t)p - 1U);
            /* update divisor count: multiply by (exp+1) */
            d = d * ((seq_t)exp + 1U);
        }
    }
    if (rem > 1)
    {
        /* rem is a prime factor > sqrt(original n) */
        phi = phi / (seq_t)rem * ((seq_t)rem - 1U);
        d   = d * 2U;
    }

    /* compute lcm = d / gcd(d,phi) * phi */
    seq_t g = _my_gcd(d, phi);
    seq_t res = d / g * phi;
    return res;
}
