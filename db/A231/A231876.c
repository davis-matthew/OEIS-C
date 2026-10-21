/*
 * A231876  Numbers k such that omega(k)^2 (cf. A001221) divides k.
 * https://oeis.org/A231876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 1100, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning natural numbers and selecting k where omega(k)^2 divides k */

#include <stdint.h>

/* Count distinct prime factors of a 64‑bit integer */
static unsigned count_distinct_prime_factors(uint64_t n)
{
    unsigned cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            ++cnt;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    /* n >= 1; search for the nth number k with omega(k)^2 | k */
    idx_t found = 0;
    uint64_t m = 1;   /* will start testing from 2 */
    while (1)
    {
        ++m;
        unsigned omega = count_distinct_prime_factors(m);
        uint64_t div = (uint64_t)omega * omega;
        if (m % div == 0)
        {
            ++found;
            if (found == n)
                return (seq_t)m;   /* conversion works for built-in and big‑int types */
        }
    }
}
