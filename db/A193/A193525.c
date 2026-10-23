/*
 * A193525  Number of even divisors of sopf(n).
 * https://oeis.org/A193525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even divisors of the sum of distinct primes
 *            dividing n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* convert index to 64‑bit unsigned */

    /* Compute sopf(n) = sum of distinct primes dividing n */
    uint64_t temp = nn;
    uint64_t sopf = 0;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            sopf += p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) sopf += temp;         /* remaining prime factor */

    /* Special case: sopf == 0 (occurs only for n = 1) */
    if (sopf == 0) return (seq_t)0;

    /* If sopf is odd, it has no even divisors */
    if ((sopf & 1) != 0) return (seq_t)0;

    /* Count divisors of sopf/2, which equals the number of even divisors of sopf */
    uint64_t m = sopf / 2;
    uint64_t tau = 1;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0) { m /= p; ++exp; }
            tau *= (exp + 1);
        }
    }
    if (m > 1) tau *= 2;                /* remaining prime factor */

    return (seq_t)tau;
}
