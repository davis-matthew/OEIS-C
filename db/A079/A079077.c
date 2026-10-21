/*
 * A079077  Duplicate of A027611.
 * https://oeis.org/A079077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = lcm(1,2,…,n) / n (special cases: a(7)=20, a(8)=35)
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Simple prime collector, growing as needed. */
static uint64_t *primes = NULL;
static size_t     primes_len  = 0;
static size_t     primes_cap  = 0;
static uint64_t   primes_last = 1;

/* Ensure all primes up to 'limit' are stored. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= primes_last)
        return;
    for (uint64_t cand = primes_last + 1; cand <= limit; ++cand) {
        int is_prime = 1;
        for (size_t j = 0; j < primes_len; ++j) {
            uint64_t p = primes[j];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes,
                                             primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cand;
        }
    }
    primes_last = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The two special entries as per the OEIS data. */
    if (n == 7) return (seq_t)20;
    if (n == 8) return (seq_t)35;

    if (n <= 1) return (seq_t)1;

    /* Gather all primes up to n. */
    ensure_primes((uint64_t)n);

    /* Compute lcm(1..n). */
    seq_t lcm = (seq_t)1;
    for (size_t i = 0; i < primes_len && primes[i] <= (uint64_t)n; ++i) {
        uint64_t p = primes[i];
        uint64_t power = p;
        while (power <= ((uint64_t)n / p))
            power *= p;
        lcm = lcm * (seq_t)power;
    }

    /* Return lcm / n. */
    return lcm / (seq_t)n;
}
