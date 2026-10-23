/*
 * A132908  Numbers formed by concatenating 8 consecutive prime numbers.
 * https://oeis.org/A132908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concat of 8 consecutive primes starting at the n‑th prime.
   a(n) = concat(p_n, p_{n+1}, …, p_{n+7})   where p_k is the k‑th prime. */

/*  The code builds a prime sieve on demand and reuses the list
    for subsequent calls.  It works for any seq_t (including an
    arbitrary‑precision integer class) and for any idx_t
    (uint64_t).  The implementation uses only C integer arithmetic
    operators and the permitted utilities.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* global state for prime cache */
static bool primes_built = false;
static uint64_t *prime_list = NULL;     /* array of primes, 1‑based in logical sense */
static size_t prime_cnt = 0;            /* number of primes stored */
static idx_t primes_limit_val = 0;      /* highest number sieved (inclusive) */

/* build a list of primes up to and including `limit` */
static void build_primes_upto_val(idx_t limit)
{
    /* free old list if any */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt = 0;
    }

    if (limit < 2) {
        primes_built = true;
        primes_limit_val = limit;
        return;
    }

    size_t l = (size_t)limit;
    bool *is_prime = (bool*)calloc(l + 1, sizeof(bool));
    if (!is_prime) exit(1);

    for (size_t i = 2; i <= l; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= l; ++i)
        if (is_prime[i]) {
            for (size_t j = i * i; j <= l; j += i)
                is_prime[j] = false;
        }

    /* count primes */
    for (size_t i = 2; i <= l; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= l; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

    primes_built = true;
    primes_limit_val = limit;
    free(is_prime);
}

/* ensure at least `need` primes are available in the global list */
static void ensure_primes_count(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;                     /* already sufficient */

    /* start with a reasonable limit if nothing built yet */
    idx_t limit = primes_built ? primes_limit_val : (idx_t)200000;

    /* double the limit until enough primes are generated */
    while (prime_cnt < (size_t)need) {
        limit = limit * 2 + 200;     /* rough growth */
        build_primes_upto_val(limit);
    }
}

/* --------------------------------------------------------------- */
/* sequence definition                                             */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for zero‑based indices */

    /* we need primes from index n to n+7 inclusive (1‑based) */
    ensure_primes_count(n + 7);

    /* pointer to the first required prime (n‑th prime) */
    const uint64_t *p = prime_list + (size_t)(n - 1);
    seq_t result = 0;

    for (size_t i = 0; i < 8; ++i) {
        uint64_t prime = p[i];

        /* count decimal digits of prime */
        uint64_t tmp = prime;
        unsigned int digits = 0;
        do {
            ++digits;
            tmp /= 10;
        } while (tmp != 0);

        /* compute 10^digits as seq_t */
        seq_t pow10 = 1;
        for (unsigned int d = 0; d < digits; ++d)
            pow10 = pow10 * 10;

        /* concatenate */
        result = result * pow10 + (seq_t)prime;
    }

    return result;
}
