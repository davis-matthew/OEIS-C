/*
 * A119257  A permutation of the positive integers formed by reversing the
 *          order of the composites within each run of composites (1 and
 *          primes are left alone).
 * https://oeis.org/A119257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes OEIS A119257. * Each maximal run of
 *            composite numbers is reversed; 1 and primes remain unchanged. *
 *            For n > 1: * if n is prime → a(n) = n; * else let p be the
 *            largest prime < n and q be the smallest prime > n. * The
 *            composite run is [p+1, q-1]; a(n) = p+1 + (q-1 - n)
 * Range:     u8 n <= 251, u16 n <= 9474, u32 n <= 9474, u64 n <= 9474,
 *            u128 n <= 9474, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cached prime sieve (1 = prime, 0 = composite) */
static uint8_t* prime_sieve = NULL;
static size_t sieve_limit = 0;

/* enlarge sieve to at least 'limit' */
static void ensure_prime_sieve(size_t limit)
{
    if (sieve_limit >= limit)
        return;
    size_t new_limit = limit;
    if (new_limit < 1000)
        new_limit = 1000;
    uint8_t* new_tbl = (uint8_t*)malloc(new_limit + 1);
    if (!new_tbl)
        abort();
    for (size_t i = 0; i <= new_limit; ++i)
        new_tbl[i] = 1;
    new_tbl[0] = new_tbl[1] = 0;
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_tbl[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                new_tbl[j] = 0;
        }
    }
    free(prime_sieve);
    prime_sieve = new_tbl;
    sieve_limit = new_limit;
}

/* smallest prime greater than n */
static idx_t next_prime_gt(idx_t n)
{
    idx_t x = n + 1;
    while (true) {
        ensure_prime_sieve((size_t)x + 100);
        if (prime_sieve[(size_t)x])
            return x;
        ++x;
    }
}

/* largest prime less than n (assumes n > 2) */
static idx_t prev_prime_lt(idx_t n)
{
    if (n <= 2)
        return 2;          /* dummy, not used for prime case */
    idx_t x = n - 1;
    while (true) {
        if (prime_sieve[(size_t)x])
            return x;
        if (x <= 2)
            return 2;
        --x;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined, but safe */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    ensure_prime_sieve((size_t)n + 100);

    if (prime_sieve[(size_t)n])   /* prime or 1 stays the same */
        return (seq_t)n;

    /* n is composite */
    idx_t prev_p = prev_prime_lt(n);
    idx_t next_q = next_prime_gt(n);

    idx_t run_start = prev_p + 1;
    idx_t run_end   = next_q - 1;

    seq_t offset = (seq_t)n - (seq_t)run_start;
    seq_t val    = (seq_t)run_end - offset;
    return val;
}
