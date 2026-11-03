/*
 * A090342  Difference between the sums of the prime factors, including
 *          multiplicity, of n and those of n + 3.
 * https://oeis.org/A090342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum of prime factors of n (with multiplicity) minus sum
 *            of prime factors of n+3 (with multiplicity)
 * Range:     i8 n <= 150, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 941 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the sum of prime factors (with multiplicity) of x. */
static seq_t sumPrimeFactors(uint64_t x)
{
    seq_t sum = 0;
    /* Handle factor 2 */
    while ((x & 1ULL) == 0)
    {
        sum += 2;
        x >>= 1;
    }
    /* Test odd divisors up to sqrt(x) */
    for (uint64_t p = 3; p <= x / p; p += 2)
    {
        while (x % p == 0)
        {
            sum += (seq_t)p;
            x /= p;
        }
    }
    /* If remaining x is a prime > 1 */
    if (x > 1)
        sum += (seq_t)x;
    return sum;
}

/* OEIS A090342: a(n) = A001414(n) - A001414(n + 3) */
seq_t a(idx_t n)
{
    seq_t s1 = sumPrimeFactors((uint64_t)n);
    seq_t s2 = sumPrimeFactors((uint64_t)(n + 3));
    return s1 - s2;
}
