/*
 * A083720  Product of the primes less than the greatest prime factor of n but
 *          not dividing n.
 * https://oeis.org/A083720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the primes < [largest prime factor of n] that
 *            do NOT divide n (OEIS A083720)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Static storage for a prime list up to the current limit */
static uint32_t  *prime_list = NULL;
static size_t     prime_cnt   = 0;
static size_t     prime_cap   = 0;
static idx_t      prime_limit = 0;

/* Extend the sieve to at least 'limit' */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;          /* already ready */

    /* Allocate a fresh sieve */
    size_t n = (size_t)limit + 1;
    char *sieve = (char *)malloc(n);
    if (!sieve) exit(1);                       /* out of memory */
    for (size_t i = 0; i < n; ++i) sieve[i] = 0;
    sieve[0] = sieve[1] = 1;                   /* 0 and 1 are not prime */

    /* Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;

    /* Resize prime array if necessary */
    if (cnt > prime_cap) {
        uint32_t *tmp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
        if (!tmp) exit(1);
        free(prime_list);
        prime_list = tmp;
        prime_cap  = cnt;
    }

    /* Fill array */
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            prime_list[prime_cnt++] = (uint32_t)i;

    free(sieve);
    prime_limit = limit;
}

/* a(n) implementation */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1, and a(0) is irrelevant */

    /* Prime factors of n (unique) */
    ensure_primes(n);                     /* we need primes up to n for factorisation */
    idx_t tmp = n;
    uint32_t pf[64];                      /* n <= 10^4 → 64 slots are more than enough */
    size_t pf_cnt = 0;

    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= tmp; ++i)
    {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            pf[pf_cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)              /* remaining factor is prime */
        pf[pf_cnt++] = (uint32_t)tmp;

    /* Largest prime factor */
    uint32_t largest = 0;
    for (size_t i = 0; i < pf_cnt; ++i)
        if (pf[i] > largest) largest = pf[i];

    /* Product of primes < largest that are NOT in the factor list */
    ensure_primes(largest - 1);           /* primes up to largest‑1 needed */
    seq_t prod = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && prime_list[i] < largest; ++i)
    {
        uint32_t p = prime_list[i];
        bool divides = false;
        for (size_t j = 0; j < pf_cnt; ++j)
            if (pf[j] == p) { divides = true; break; }
        if (!divides) prod *= (seq_t)p;
    }

    return prod;
}
