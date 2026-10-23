/*
 * A169802  Numbers k equal to A074036(k) = sum of primes from least to
 *          largest prime factor.
 * https://oeis.org/A169802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The n‑th term a(n) is the n‑th positive integer k such that
 *            A074036(k) = sum of all primes from the least prime factor of k
 *            to the greatest prime factor of k (inclusive)
 * Range:     u8 n <= 57, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   lower_bound_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/lower_bound_prime.h"

/* ------------------------------------------------------------------------- */
/*  Prime sieve  (Eratosthenes) – up to a fixed limit                        */
/* ------------------------------------------------------------------------- */

static uint32_t *primes   = NULL;   /* primes[i]  (uint32_t is enough for our limit) */
static size_t   prime_cnt = 0;      /* number of primes found                 */
static seq_t  *prime_sum = NULL;    /* prefix sums of primes                  */

static void build_primes(void)
{
    if (primes) return;            /* already built */

    /* Limit chosen large enough to cover the first 10 000 terms. */
    const uint64_t LIMIT = 20000000ULL;   /* 20 million  */
    const size_t   size  = (size_t)LIMIT + 1;

    uint8_t *is_prime = (uint8_t *)malloc(size);
    if (!is_prime) abort();
    for (size_t i = 0; i < size; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    uint64_t sqrt_limit = (uint64_t)((double)LIMIT * 0.5 + 1);

    for (uint64_t p = 2; p <= sqrt_limit; ++p) {
        if (!is_prime[p]) continue;
        for (uint64_t m = p * p; m <= LIMIT; m += p)
            is_prime[m] = 0;
    }

    /* First count primes to allocate array of correct size */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);

    /* Prefix sums of primes */
    prime_sum = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    if (!prime_sum) abort();
    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        sum += (seq_t)primes[i];
        prime_sum[i] = sum;
    }
}

/* ------------------------------------------------------------------------- */
/*  Binary search to find the first index with primes[i] >= val               */

/* ------------------------------------------------------------------------- */
/*  Find least and greatest prime factor of a composite or prime number      */
/* ------------------------------------------------------------------------- */
static void find_lpf_gpf(uint64_t k, uint32_t *lpf, uint32_t *gpf)
{
    /* If k is prime, lpf = gpf = k */
    if (k <= 2) {
        *lpf = *gpf = (uint32_t)k;
        return;
    }

    uint64_t n = k;
    uint32_t lpf_val = 0, gpf_val = 0;

    /* Iterate over primes up to sqrt(n) */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            if (!lpf_val) lpf_val = p;
            gpf_val = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {                /* remaining factor is prime */
        if (!lpf_val) lpf_val = (uint32_t)n;
        if (n > gpf_val) gpf_val = (uint32_t)n;
    }

    *lpf = lpf_val;
    *gpf = gpf_val;
}

/* ------------------------------------------------------------------------- */
/*  Compute the sum of primes between lpf and gpf (inclusive).               */
/* ------------------------------------------------------------------------- */
static seq_t sum_primes_range(uint32_t lpf, uint32_t gpf)
{
    size_t idx_l = lower_bound_prime(primes, prime_cnt, lpf);
    size_t idx_g = lower_bound_prime(primes, prime_cnt, gpf);

    seq_t res = prime_sum[idx_g];
    if (idx_l > 0) res -= prime_sum[idx_l - 1];
    return res;
}

/* ------------------------------------------------------------------------- */
/*  Dynamic array to store discovered terms of the sequence                 */
/* ------------------------------------------------------------------------- */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_size = 0;
static uint64_t last_checked = 1;          /* last integer examined */

static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms   = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------------- */
/*  Main function to return the nth term of the sequence                     */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below offset irrelevant */

    /* Ensure primes and prefix sums built */
    if (!primes) build_primes();

    /* Find terms until we have n of them */
    while (terms_size < n) {
        ++last_checked;
        uint64_t k = last_checked;

        /* Compute least and greatest prime factor */
        uint32_t lpf, gpf_val;
        find_lpf_gpf(k, &lpf, &gpf_val);

        seq_t term_sum = sum_primes_range(lpf, gpf_val);

        if ((seq_t)k == term_sum) {
            grow_terms(terms_size + 1);
            terms[terms_size++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
