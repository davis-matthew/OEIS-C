/*
 * A096277  Sum of successive sums of successive primes: a(n) = s(n) + s(n+1)
 *          where s(n) = prime(n) + prime(n+1) (A001043).
 * https://oeis.org/A096277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + 2*prime(n+1) + prime(n+2) (OEIS A096277)
 * Range:     u8 n <= 17, u16 n <= 1898, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* global static state for primes */
static uint64_t *prime_tbl = NULL;     /* array of primes */
static size_t    prime_cnt = 0;        /* number of primes stored */
static size_t    prime_lim = 0;        /* current sieve limit */

/* ensure we have at least (need) primes (need >= n+2) */
static void ensure_primes(idx_t need)
{
    if (prime_tbl && prime_cnt >= need) return;   /* already enough */

    /* rough upper bound for the need-th prime: 15 * need + 100 is safe for
       need up to at least 1,000,000 (the 1,000,000th prime ≈ 15,485,863) */
    size_t limit = need * 15 + 100;
    if (limit < 20) limit = 20;   /* small safety */

    /* sieve up to 'limit' */
    uint8_t *sieve = (uint8_t *)malloc(limit + 1);
    if (!sieve) abort();          /* allocation failure */
    memset(sieve, 1, limit + 1);
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    /* collect primes up to 'need' */
    uint64_t *new_tbl = (uint64_t *)realloc(prime_tbl, need * sizeof(uint64_t));
    if (!new_tbl) abort();
    prime_tbl = new_tbl;

    size_t idx = 0;
    for (size_t i = 2; i <= limit && idx < need; ++i)
        if (sieve[i])
            prime_tbl[idx++] = i;

    free(sieve);

    prime_cnt = idx;
    prime_lim = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1; return 0 */

    /* we need primes up to index n+2 (1-based) */
    size_t need = (size_t)(n + 2);           /* primes[0] = prime(1) */
    ensure_primes(need);

    /* indices in prime_tbl are 0-based */
    seq_t p1 = (seq_t)prime_tbl[(size_t)(n - 1)];   /* prime(n)   */
    seq_t p2 = (seq_t)prime_tbl[(size_t)n];         /* prime(n+1) */
    seq_t p3 = (seq_t)prime_tbl[(size_t)(n + 1)];   /* prime(n+2) */

    return p1 + (seq_t)2 * p2 + p3;
}
