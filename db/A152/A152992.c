/*
 * A152992  a(n) = sigma(n) - d(n) - pi(n).
 * https://oeis.org/A152992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) - d(n) - pi(n)
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(n)  = sum of divisors of n          */
/* d(n)      = number of divisors of n       */
/* pi(n)     = number of primes <= n         */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* The build supplies the types idx_t and seq_t.  Do NOT redefine them. */

/* Utility to generate all primes up to a limit and return the count.  */
/* The function is provided by the framework and allocates an array
   that we must free with free() when we replace it. */
#include "../utils/build_primes_upto.h"

/* Static storage for the dynamic prime table.  */
static size_t prime_limit = 0;     /* largest number for which table is built   */
static uint64_t *prime_table = NULL; /* primes stored in increasing order      */
static size_t prime_count = 0;        /* number of primes in prime_table         */

/* Extend the prime table to include all primes <= n.  */
static void ensure_prime_table(idx_t n)
{
    if (n <= prime_limit)
        return; /* already built enough */

    /* Allocate a new table up to n.  */
    uint64_t *new_table;
    size_t new_count;
    build_primes_upto((idx_t)n, &new_table, &new_count);

    /* Free the old table and replace it.  */
    free(prime_table);
    prime_table = new_table;
    prime_limit = (size_t)n;
    prime_count = new_count;
}

/* Compute sigma(n), the sum of all positive divisors of n, */
static seq_t sigma_of(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = 1;
    idx_t remaining = n;
    size_t i = 0;

    /* Iterate over primes up to sqrt(remaining).  */
    while (i < prime_count)
    {
        uint64_t p = prime_table[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)remaining)
            break;

        if (remaining % p == 0) {
            /* Count the exponent e of prime p in n.  */
            unsigned e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }

            /* term = (p^(e+1)-1)/(p-1) */
            seq_t pow = 1;
            for (unsigned j = 0; j < e + 1; ++j)
                pow *= (seq_t)p;
            seq_t term = (pow - 1) / ((seq_t)p - 1);
            result *= term;
        }
        ++i;
    }

    /* If a prime factor > sqrt(n) remains, it occurs exactly once.  */
    if (remaining > 1)
        result *= ((seq_t)remaining + 1);

    return result;
}

/* Compute d(n), the number of divisors of n, */
static seq_t divisor_count_u64(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t count = 1;
    idx_t remaining = n;
    size_t i = 0;

    while (i < prime_count)
    {
        uint64_t p = prime_table[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)remaining)
            break;
        if (remaining % p == 0) {
            unsigned e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            count *= (seq_t)(e + 1);
        }
        ++i;
    }

    if (remaining > 1)
        count *= (seq_t)2; /* remaining prime factor */

    return count;
}

/* Compute pi(n), the prime counting function.  */
static seq_t prime_pi(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    return (seq_t)prime_count;
}

/* The requested sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset is 1 */

    ensure_prime_table(n);

    seq_t sigma = sigma_of(n);
    seq_t d     = divisor_count_u64(n);
    seq_t pi    = prime_pi(n);

    return sigma - d - pi;
}
