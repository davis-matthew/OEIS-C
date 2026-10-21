/*
 * A065988  a(n) is the largest prime p < n such that n mod p is a perfect
 *          square; a(n) = 0 if no such prime exists.
 * https://oeis.org/A065988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest prime p < n such that n mod p is a perfect
 *            square; a(n) = 0 if no such prime exists
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for 64‑bit unsigned numbers */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;   /* sqrt(2^64‑1) = 2^32‑1 */
    while (lo <= hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n = 1 or 2 => no prime < n except the trivial 2 case; return 0 */
    if (n <= 2)
        return 0;

    /* Allocate sieve array of n bytes (one byte per integer up to n-1) */
    char *sieve = (char *)malloc((size_t)n);
    if (!sieve)
        return 0;                       /* worst‑case fallback */

    for (size_t i = 0; i < (size_t)n; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;             /* 0 and 1 are not prime */

    /* Sieve of Eratosthenes up to n-1 */
    for (idx_t i = 2; (unsigned long long)i * i < n; ++i)
    {
        if (sieve[(size_t)i])
        {
            for (idx_t j = i * i; j < n; j += i)
                sieve[(size_t)j] = 0;
        }
    }

    seq_t result = 0;                    /* default when no suitable prime found */

    /* Search for the largest prime < n in descending order */
    for (idx_t p = n - 1; p > 1; --p)
    {
        if (!sieve[(size_t)p])
            continue;                    /* not prime */

        uint64_t r = (uint64_t)(n % p);   /* n < p? no, p < n */
        uint64_t k = isqrt_u64(r);
        if (k * k == r)                  /* n mod p is a perfect square */
        {
            result = (seq_t)p;           /* largest found so far */
            break;                       /* no larger prime remains */
        }
    }

    free(sieve);
    return result;
}
