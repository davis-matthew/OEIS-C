/*
 * A062792  Moebius transform of primorial numbers.
 * https://oeis.org/A062792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d)·primorial(n/d) (OEIS A062792)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   approximate_prime_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/approximate_prime_limit.h"

/* ------------------------------------------------------------------ */
/* Global state:  a table of the first primes and a cache of primorials */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list  = NULL;   /* prime_list[0] = 2 */
static size_t   prime_count  = 0;      /* number of primes stored */

static seq_t *primorial_cache = NULL;   /* primorial_cache[0] = 1 */
static size_t primorial_cap   = 0;      /* number of primorials stored */

/* ------------------------------------------------------------------ */
/* Rough upper bound on the n‑th prime (Rosser 1938)                     */

/* ------------------------------------------------------------------ */
/* Eratosthenes sieve up to 'limit' – fills prime_list                */
/* ------------------------------------------------------------------ */
static void generate_primes_up_to(size_t limit)
{
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);

    /* initialise */
    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    /* sieve */
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = false;

    /* count primes */
    size_t new_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++new_count;

    /* store them */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_count * sizeof(uint64_t));
    if (!new_list) exit(EXIT_FAILURE);
    prime_list = new_list;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint64_t)i;
    prime_count = new_count;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Ensure we have at least 'n' primes available                         */
/* ------------------------------------------------------------------ */
static void ensure_primes_upto(size_t n)
{
    if (prime_count >= n) return;
    size_t limit = approximate_prime_limit(n);
    generate_primes_up_to(limit);
    if (prime_count < n)             /* a very unlikely case – extend further */
        ensure_primes_upto(n);
}

/* ------------------------------------------------------------------ */
/* Ensure we have primorials up to index 'n'                           */
/* ------------------------------------------------------------------ */
static void ensure_primorial_up_to(size_t n)
{
    if (primorial_cap > n) return;

    if (primorial_cache == NULL) {
        primorial_cache = (seq_t *)realloc(primorial_cache, (n + 1) * sizeof(seq_t));
        if (!primorial_cache) exit(EXIT_FAILURE);
        primorial_cache[0] = (seq_t)1;
        primorial_cap = 1;
    } else {
        primorial_cache = (seq_t *)realloc(primorial_cache, (n + 1) * sizeof(seq_t));
        if (!primorial_cache) exit(EXIT_FAILURE);
    }

    for (size_t i = primorial_cap; i <= n; ++i)
        primorial_cache[i] = primorial_cache[i - 1] * (seq_t)prime_list[i - 1];
    primorial_cap = n + 1;
}

/* ------------------------------------------------------------------ */
/* Möbius function for 64‑bit integers                                 * 
/*  (returns -1, 0, or 1)                                             */
/* ------------------------------------------------------------------ */
static int mobius_of(idx_t d)
{
    if (d == 1) return 1;

    int sign = 1;
    for (size_t i = 0; i < prime_count &&
                       (idx_t)prime_list[i] * (idx_t)prime_list[i] <= d; ++i) {
        uint64_t p   = prime_list[i];
        int      cnt = 0;
        while (d % p == 0) {
            d   /= p;
            ++cnt;
            if (cnt > 1) return 0;                /* squared prime factor */
        }
        if (cnt == 1) sign = -sign;
    }
    if (d > 1) sign = -sign;                       /* remaining prime factor */
    return sign;
}

/* ------------------------------------------------------------------ */
/* OEIS sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* below offset – never used */

    /* Primes and primorials must cover index n */
    ensure_primes_upto((size_t)n);
    ensure_primorial_up_to((size_t)n);

    seq_t pos = 0, neg = 0;     /* separate positives/negatives to avoid overflow */

    /* Enumerate divisors via sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            int mu = mobius_of(d);
            if (mu != 0) {
                idx_t k = n / d;                 /* index for primorial */
                seq_t val = primorial_cache[k];
                if (mu > 0) pos += val; else neg += val;
            }

            /* paired divisor e = n/d, if distinct */
            idx_t e = n / d;
            if (e != d) {
                int mu = mobius_of(e);
                if (mu != 0) {
                    idx_t k = n / e;                 /* equals d */
                    seq_t val = primorial_cache[k];
                    if (mu > 0) pos += val; else neg += val;
                }
            }
        }
    }
    return pos - neg;    /* difference of accumulated sums */
}
