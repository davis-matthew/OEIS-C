/*
 * A100573  Smallest difference between distinct prime divisors of n, or 0 if
 *          n is a prime power.
 * https://oeis.org/A100573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 525, u16 n <= 9714, u32 n <= 9714, u64 n <= 9714,
 *            u128 n <= 9714, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A100573: smallest difference between distinct prime divisors
 * of n, or 0 if n is a prime power. Implementation uses trial‑division
 * factorisation. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t nn = n;
    uint64_t primes[8];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= nn; ++p)
    {
        if (nn % p == 0)
        {
            primes[cnt++] = p;
            while (nn % p == 0)
                nn /= p;
        }
    }
    if (nn > 1)          // remaining prime factor
        primes[cnt++] = nn;

    if (cnt <= 1)
        return 0;

    int min_diff = -1;
    for (int i = 0; i < cnt; ++i)
    {
        for (int j = i + 1; j < cnt; ++j)
        {
            int diff = (int)(primes[j] - primes[i]);
            if (min_diff == -1 || diff < min_diff)
                min_diff = diff;
        }
    }
    return (seq_t)min_diff;
}
