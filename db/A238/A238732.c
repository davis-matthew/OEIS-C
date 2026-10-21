/*
 * A238732  Number of primes p < n with floor((n-p)/3) a square.
 * https://oeis.org/A238732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = #{ primes p < n | floor((n - p) / 3) is a perfect square } */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64 works on idx_t (uint64_t) */

static uint32_t *g_primes   = NULL;   /* array of primes up to current sieve limit */
static size_t   g_prime_cnt = 0;      /* number of primes stored */
static idx_t    g_sieve_lim = 0;      /* current maximum sieve limit */

/* Expand the sieve of Eratosthenes to cover all numbers up to 'limit' (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_sieve_lim)
        return;                       /* already up to date */

    /* Allocate a temporary boolean array for the sieve */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return;                       /* allocation failed – leave previous state unchanged */

    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* Count how many primes were found to allocate the vector */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++new_cnt;

    uint32_t *new_primes = (uint32_t *)malloc(new_cnt * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return;                       /* allocation failed – leave previous state unchanged */
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            new_primes[pos++] = (uint32_t)i;

    free(is_prime);

    /* Replace old prime list with the new one */
    free(g_primes);
    g_primes   = new_primes;
    g_prime_cnt = new_cnt;
    g_sieve_lim = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;          /* no primes < n */

    /* Ensure we have all primes below n */
    ensure_primes(n - 1);

    seq_t count = 0;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        idx_t p = (idx_t)g_primes[i];
        if (p >= n)
            break;

        idx_t m = (n - p) / 3;
        idx_t s = _floor_isqrt_u64(m);
        if (s * s == m)
            ++count;
    }

    return count;
}
