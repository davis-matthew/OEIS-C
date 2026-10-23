/*
 * A102067  Numbers k such that k does not divide P(k)! even though P(k)^2 is
 *          not a factor of k, where P(k) is the largest prime factor of k.
 * https://oeis.org/A102067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 678, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_in_fact, push_seq, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102067: numbers k such that the largest prime factor p of k satisfies
 * p^2 ∤ k  and   k ∤ p!.
 * The function a(n) returns the n‑th such integer (a(1)=12, a(2)=24, …)
 * by iterating upward through candidates, testing the conditions,
 * and caching the found terms.  This works for any idx_t≥1 in
 * arbitrary order and for the usual 64‑bit build. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions provided by the build system */
#include "../utils/push_seq.h"
#include "../utils/simple_sieve.h"
#include "../utils/exp_in_fact.h"

/* --------------------------------------------------------------------- */
/* Prime sieve (executed once) ----------------------------------------- */
static uint32_t *all_primes = NULL;   /* array of primes up to LIMIT */
static size_t    prime_cnt  = 0;      /* number of primes found */

static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    /* 2 000 000 is ample for candidates up to a few million. */
    const uint32_t LIMIT = 2000000U;
    uint32_t *tmp = NULL;
    size_t   ct  = 0;
    simple_sieve(LIMIT, &tmp, &ct);

    all_primes = tmp;
    prime_cnt  = ct;
    done = true;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Test whether a given integer k satisfies the defining condition. ----*/
static bool qualifies(uint64_t k)
{
    uint64_t temp = k;

    /* Store distinct prime factors and their exponents. */
    uint32_t fac_primes[20];
    uint32_t fac_exps[20];
    size_t fac_cnt = 0;

    uint32_t largest_prime = 0;
    uint32_t largest_exp   = 0;

    /* Trial division using the sieve primes. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = all_primes[i];
        if ((uint64_t)p * p > temp) break;

        if (temp % p == 0) {
            uint32_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            fac_primes[fac_cnt] = p;
            fac_exps[fac_cnt++] = cnt;

            if (p > largest_prime) {
                largest_prime = p;
                largest_exp   = cnt;
            }
        }
    }

    /* Remaining factor > √k, if any, is prime. */
    if (temp > 1) {
        uint32_t p = (uint32_t)temp;
        fac_primes[fac_cnt] = p;
        fac_exps[fac_cnt++] = 1;
        if (p > largest_prime) {
            largest_prime = p;
            largest_exp   = 1;
        }
    }

    /* Condition (1): largest prime must appear only once. */
    if (largest_exp != 1) return false;

    /* Condition (2): at least one other prime factor has exponent
     * exceeding its exponent in largest_prime!. */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t q   = fac_primes[i];
        uint32_t exp = fac_exps[i];

        if (q == largest_prime) continue; /* already verified exp==1 */

        uint32_t fact_exp = exp_in_fact(largest_prime, q);
        if (exp > fact_exp) return true;
    }

    return false;
}

/* --------------------------------------------------------------------- */
/* Cached sequence storage -------------------------------------------- */
static seq_t  *cache = NULL;     /* dynamic array of found terms       */
static idx_t   cap   = 0;        /* current capacity                  */
static idx_t   len   = 0;        /* number of stored terms             */
static uint64_t next_candidate = 12; /* first number to test */

/* --------------------------------------------------------------------- */
/* Public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise once. */
    static bool init_done = false;
    if (!init_done) {
        init_primes();
        init_done = true;
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        if (qualifies(next_candidate)) {
            push_seq(&cache, &cap, &len, (seq_t)next_candidate);
        }
        ++next_candidate;
    }

    /* Return the n‑th term (1‑based). */
    return cache[n - 1];
}
