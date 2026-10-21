/*
 * A068958  Smallest value of k such that p | (10^k + 1), where p is the n-th
 *          prime; or 0 if no such k exists.
 * https://oeis.org/A068958
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the nth prime p divides 10^k + 1;
 *            a(n) = 0 if no such k exists. (Computed by determining the
 *            multiplicative order of 10 modulo p.)
 * Range:     u8 n <= 57, u16 n <= 6920, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Prime storage --------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* array of discovered primes   */
static size_t   prime_cnt  = 0;       /* number of primes stored      */
static size_t   prime_cap  = 0;       /* current allocation capacity   */
static uint64_t next_cand  = 2;        /* next candidate to test       */

/* Ensure that at least n primes are available in prime_list           */
static void
ensure_primes(idx_t n)
{
    /* If we already have enough primes do nothing. */
    if (prime_cnt >= (size_t)n)
        return;

    /* Grow the list until it contains at least n primes. */
    while (prime_cnt < (size_t)n) {
        uint64_t cand = next_cand;
        /* Prepare the next candidate: after 2 only odd numbers. */
        next_cand = (cand == 2ULL) ? 3ULL : cand + 2ULL;

        bool is_prime = true;
        if (cand < 2ULL) {
            is_prime = false;
        } else if (cand == 2ULL) {
            is_prime = true;
        } else {
            for (size_t i = 0; i < prime_cnt &&
                 prime_list[i] <= cand / prime_list[i]; ++i) {
                if (cand % prime_list[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) {
                    /* allocation failed – keep the old array, cannot continue */
                    return;
                }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* Factor n into distinct prime factors; write them into `factors`      */
/* and store the count in *cnt.                                          */
static void
factor_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= n / prime_list[i];
         ++i) {
        if (n % prime_list[i] == 0) {
            factors[(*cnt)++] = prime_list[i];
            while (n % prime_list[i] == 0)
                n /= prime_list[i];
        }
    }
    if (n > 1ULL) {
        factors[(*cnt)++] = n;
    }
}

/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Offset of the sequence is 4: a(4) is the first listed term. */
    if (n < 4ULL)
        return 0;

    /* Make sure we have the nth prime available. */
    ensure_primes(n);
    uint64_t p = prime_list[(size_t)(n - 1)]; /* n-th prime (1‑based) */

    /* 10 is not coprime to p for p = 2 or 5 ⇒ no solution. */
    if (p == 2ULL || p == 5ULL)
        return 0;

    /* Compute the multiplicative order of 10 modulo p. */
    uint64_t ord = p - 1ULL;                       /* order divides p-1      */
    uint64_t fac[64];
    size_t   fcnt;
    factor_distinct(ord, fac, &fcnt);

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = fac[i];
        /* Reduce the order as long as the test passes. */
        while (ord % q == 0) {
            uint64_t test = ord / q;
            if (mod_pow((seq_t)10, test, (seq_t)p) == (seq_t)1)
                ord = test;
            else
                break;
        }
    }

    /* If the order is odd, the congruence 10^k ≡ –1 has no solution. */
    if (ord & 1ULL)
        return 0;

    /* Minimal k is ord/2. */
    return (seq_t)(ord / 2ULL);
}
