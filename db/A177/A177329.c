/*
 * A177329  Number of factors in the representation of n! as a product of
 *          distinct terms of A050376.
 * https://oeis.org/A177329
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over all primes p≤n of popcount( exponent of p in n!
 *            ) where exponent = Σ_{k≥1} ⌊n / p^k⌋
 * Range:     u8 n <= 954, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches the list of primes up to the largest n seen so far.
   It works for any seq_t type: native or arbitrary‑precision. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* ----- Prime sieve utilities ----- */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static idx_t    primes_limit = 0;        /* largest n for which primes up to n are known */

/* Ensure that primes[0..primes_cnt-1] contains all primes ≤ n. */
static void
ensure_primes(idx_t n)
{
    if ((idx_t)primes_limit >= n)
        return;                           /* already have enough primes */

    /* Allocate temporary sieve array */
    size_t sz = (size_t)n + 1;             /* indices 0 … n */
    bool *is_prime = (bool*)malloc(sz);
    if (!is_prime)
        abort();                          /* out of memory */
    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= (size_t)n; ++i) {
        if (!is_prime[i])
            continue;
        for (size_t j = i * i; j <= (size_t)n; j += i)
            is_prime[j] = 0;
    }

    /* Count the primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i])
            ++cnt;

    /* Reallocate the primes array to the new size */
    uint64_t *new_primes = (uint64_t*)realloc(primes, cnt * sizeof(uint64_t));
    if (!new_primes && cnt > 0)          /* realloc failed only if cnt>0 */
        abort();
    primes = new_primes;
    primes_cnt = cnt;

    /* Fill the primes array */
    size_t p = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i])
            primes[p++] = (uint64_t)i;

    free(is_prime);
    primes_limit = n;
}

/* ----- Sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* offset starts at 2 */

    ensure_primes(n);

    seq_t sum = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)n)
            break;

        /* Compute exponent of p in n! using Legendre's formula */
        uint64_t e = 0;
        uint64_t div = p;
        while (div <= (uint64_t)n) {
            e += (n / div);
            if (div > (uint64_t)n / p)          /* avoid overflow of div*p */
                break;
            div *= p;
        }

        /* Count 1-bits in e */
        uint64_t bits = 0;
        uint64_t v = e;
        while (v) {
            bits += v & 1;
            v >>= 1;
        }

        sum = sum + (seq_t)bits;
    }

    return sum;
}
