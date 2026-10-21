/*
 * A080773  In binary representation: sum of number of 1's in prime factors of
 *          n (with repetition).
 * https://oeis.org/A080773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∑_{p^e‖n} e * popcount(p)
 *           (totally additive with a(p) = popcount(p)).
 */
/* Ensure the usual integer types are available */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global state to cache primes up to the largest n requested. */
static size_t primes_cnt = 0;
static idx_t *primes = NULL;
static size_t primes_cap = 0;
static bool *is_prime = NULL;
static size_t is_prime_cap = 0;
static idx_t last_limit = 0;

/* Extend the prime sieve and prime list so that all primes ≤ limit are prepared. */
static void extend_to(idx_t limit)
{
    if (limit <= last_limit)
        return;

    /* Allocate / resize the is_prime array. */
    size_t new_cap = (size_t)(limit + 1);
    bool *old = is_prime;
    is_prime = (bool *)realloc(is_prime, new_cap * sizeof(bool));

    /* Initialize the new entries to true. */
    for (size_t i = (size_t)(last_limit == 0 ? 2 : last_limit + 1);
         i <= (size_t)limit; ++i)
        is_prime[i] = true;

    /* 0 and 1 are not prime. */
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Simple Eratosthenes sieve up to limit. */
    for (idx_t p = 2; (p * p) <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;
        }
    }

    /* Count primes to reallocate the prime list if needed. */
    size_t new_primes = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++new_primes;

    if (new_primes > primes_cap) {
        primes_cap = new_primes;
        if (primes_cap == 0)
            primes_cap = 128;
        primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
    }

    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;

    last_limit = limit;
}

/* Main function to compute A080773. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0; /* a(1) = 0, and n=0 is out of defined range */

    /* Prepare primes up to n. */
    extend_to(n);

    seq_t result = (seq_t)0;
    idx_t temp = n;

    /* Use the sieve-generated primes for factorisation. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            /* count how many times p divides temp */
            while (temp % p == 0) {
                result += (seq_t)__builtin_popcountll((uint64_t)p);
                temp /= p;
            }
        }
    }

    /* If a prime factor larger than sqrt(n) remains. */
    if (temp > 1)
        result += (seq_t)__builtin_popcountll((uint64_t)temp);

    return result;
}
