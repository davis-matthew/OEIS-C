/*
 * A129252  Smallest prime factor p of n such that p^p is a divisor of n,
 *          a(n)=1 if no such factor exists.
 * https://oeis.org/A129252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime factor p of n such that p^p divides n;
 *            a(n)=1 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n < 2 there is no prime factor, so the answer is 1. */
    if (n < 2)
        return (seq_t)1;

    /* Only primes p with p^p <= the maximum possible n fit in 64 bits
       (2^64-1). Those are 2,3,5,7,11,13. */
    static const uint64_t small_primes[6] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};

    uint64_t n_val = (uint64_t)n;               /* work in 64‑bit arithmetic */

    for (size_t i = 0; i < 6; ++i)
    {
        uint64_t p = small_primes[i];

        if (n_val % p != 0)
            continue;                          /* p is not a divisor of n */

        /* Count exponent of p in n */
        uint64_t temp = n_val;
        int exponent = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exponent;
        }

        /* If the exponent is at least p, p^p divides n.  Since we iterate
           primes in increasing order, the first hit is the smallest such
           prime factor. */
        if (exponent >= (int)p)
            return (seq_t)p;
    }

    /* No prime factor p with p^p | n was found. */
    return (seq_t)1;
}
