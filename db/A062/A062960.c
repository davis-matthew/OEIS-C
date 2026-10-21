/*
 * A062960  Number of divisors of (n!)^n (A036740).
 * https://oeis.org/A062960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{p ≤ n} (n·e_p + 1) where e_p = Σ_{k ≥ 1} ⌊n / p^k⌋ is
 *            the exponent of p in n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence gives the number of divisors of (n!)^n for n ≥ 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of primes up to the largest limit requested. */
static uint64_t *primes = NULL;    /* array of prime values */
static size_t   prime_cnt = 0;     /* number of primes cached */
static idx_t    primes_limit = 0;  /* maximum index for which primes are cached */

/* ------------------------------------------------------------------ */
/* Build a sieve of Eratosthenes up to 'limit' (inclusive) and cache   */
/* the resulting primes in the static array. If a larger limit is     */
/* requested than already cached, the array is grown accordingly.     */
/* ------------------------------------------------------------------ */
static void prime_sieve_upto(idx_t limit)
{
    /* If we already have enough primes, nothing to do. */
    if (limit <= primes_limit)
        return;

    /* Allocate a temporary boolean array for the sieve. */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime)
        exit(1);                 /* allocation failure */

    /* Mark all as prime initially. */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;

    /* 0 and 1 are not prime. */
    if (limit >= 0)
        is_prime[0] = false;
    if (limit >= 1)
        is_prime[1] = false;

    /* Sieve: for each candidate i, cross out multiples. */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* Count how many primes we found to allocate an array. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* Allocate a new array to hold the primes. */
    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes)
        exit(1);

    /* Copy primes into the new array. */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            new_primes[pos++] = (uint64_t)i;

    /* Clean up and update the static cache. */
    free(primes);
    free(is_prime);
    primes     = new_primes;
    prime_cnt  = cnt;
    primes_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the first two values explicitly. */
    if (n <= 1)
        return (seq_t)1;

    /* Ensure primes up to n are available. */
    prime_sieve_upto(n);

    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = primes[idx];
        if ((idx_t)p > n)
            break;

        /* Compute exponent of p in n! using Legendre's formula. */
        idx_t power = (idx_t)p;
        idx_t exp   = 0;
        while (power <= n) {
            exp += n / power;
            if (power > n / p)
                break;               /* avoid overflow of power */
            power *= p;
        }

        /* term = n*exp + 1, all in seq_t to avoid overflow. */
        seq_t term = (seq_t)((seq_t)exp * (seq_t)n + 1);
        result *= term;
    }

    return result;
}
