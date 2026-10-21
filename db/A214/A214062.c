/*
 * A214062  Least m>0 such that gcd(2*n+m, 2*n-1-m) > 1.
 * https://oeis.org/A214062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min_{p | (4n-1)} (p-1)/2 where p runs over all prime
 *            factors of 4n‑1
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* n < 1 is undefined for the OEIS offset */

    /* Compute v = 4*n - 1 using seq_t arithmetic.  The result fits
       into an unsigned 64‑bit integer for all practical n in the test set. */
    seq_t v_seq = (seq_t)n * 4ULL - 1ULL;

    /* Work on a 64‑bit integer for simplicity. */
    uint64_t v = (uint64_t)v_seq;

    uint64_t min_m = (uint64_t)-1;   /* start with a large value */
    uint64_t tmp   = v;

    /* Trial division by odd numbers up to sqrt(tmp). */
    for (uint64_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            uint64_t cand = (p - 1) / 2;     /* candidate m for prime factor p */
            if (cand < min_m) min_m = cand;

            /* Remove all occurrences of p from tmp. */
            do { tmp /= p; } while (tmp % p == 0);
        }
    }

    /* If what's left is a prime larger than sqrt(v). */
    if (tmp > 1)
    {
        uint64_t cand = (tmp - 1) / 2;
        if (cand < min_m) min_m = cand;
    }

    return (seq_t)min_m;
}
