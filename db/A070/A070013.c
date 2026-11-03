/*
 * A070013  Number of prime factors of n divided by the number of n's distinct
 *          prime factors (rounded).
 * https://oeis.org/A070013
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(Ω(n)/ω(n)) for n ≥ 2 Ω(n) = total number of prime
 *            factors of n (with multiplicity) ω(n) = number of distinct prime
 *            factors of n Uses the helper functions bigomega() and
 *            count_distinct_prime_factors()
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t bigomega(idx_t n)
{
    /* Count prime factors of n with multiplicity. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2. */
    while ((n & 1) == 0) {
        n >>= 1;
        ++count;
    }

    /* Test odd divisors up to sqrt(n). */
    for (idx_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }

    /* If n > 1 now, it is a prime factor > sqrt(original n). */
    if (n > 1)
        ++count;

    return count;
}

static int count_distinct_prime_factors(uint64_t n)
{
    int cnt = 0;
    if (n < 2) return 0;
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* below offset – undefined, return 0 */

    seq_t big   = bigomega(n);                          /* Ω(n) */
    int  omega = count_distinct_prime_factors((uint64_t)n); /* ω(n) */

    /* rounded average: floor((Ω(n) + ω(n)/2) / ω(n)) */
    seq_t numerator = big + (seq_t)(omega / 2);
    return numerator / (seq_t)omega;
}
