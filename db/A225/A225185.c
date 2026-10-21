/*
 * A225185  Primes p which do not have a primitive root that divides p+1.
 * https://oeis.org/A225185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A225185:
   a(n) = the n-th prime p such that no primitive root of p divides p+1.
   The sequence is computed on demand and memoised.
   The implementation uses only integer arithmetic and the utilities
   provided (in particular, mod_pow).  It is valid in C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#include "../utils/mod_pow.h"

/* ---------------------------------------------------------------------- */
/* Prime list (trial‑division)                                           */
/* ---------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t    prime_cnt    = 0;       /* number of primes stored    */
static size_t    prime_cap    = 0;       /* allocated capacity         */
static uint64_t  last_checked_prime = 2;

/* Ensure prime_list has room for at least 'need' entries. */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Initialise the prime list with 2 and 3. */
static void prime_list_init(void)
{
    if (prime_list) return;              /* already initialised */
    ensure_prime_cap(2);
    prime_list[0] = 2;
    prime_list[1] = 3;
    prime_cnt    = 2;
    last_checked_prime = 3;
}

/* Find the next prime > start, adding it to prime_list. */
static uint64_t next_prime(uint64_t start)
{
    if (start < 2) start = 1;
    uint64_t candidate = start + 1;
    for (;; ++candidate) {
        uint64_t sq = (uint64_t) sqrt((double) candidate);
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > sq) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = candidate;
            last_checked_prime = candidate;
            return candidate;
        }
    }
}

/* Ensure prime_list contains all primes up to sqrt(n). */
static void ensure_primes_up_to_sqrt(uint64_t n)
{
    uint64_t limit = (uint64_t) sqrt((double) n) + 1;
    while (last_checked_prime < limit)
        next_prime(last_checked_prime);
}

/* Factorization helpers ------------------------------------------------- */
static void factor_distinct(uint64_t n, uint64_t *fac, int *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    ensure_primes_up_to_sqrt(n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) {
            fac[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) fac[(*cnt)++] = n;
}

static void factor_with_exp(uint64_t n, uint64_t *fac, int *exp, int *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    ensure_primes_up_to_sqrt(n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) {
            fac[(*cnt)] = p;
            exp[(*cnt)] = 0;
            while (n % p == 0) { n /= p; exp[(*cnt)]++; }
            (*cnt)++;
        }
    }
    if (n > 1) {
        fac[(*cnt)] = n;
        exp[(*cnt)] = 1;
        (*cnt)++;
    }
}

/* Test whether a prime p has a primitive root that divides p+1. */
static bool has_primitive_root_dividing(uint64_t p)
{
    uint64_t phi = p - 1;

    /* Prime factors of phi */
    uint64_t fac_phi[64];
    int cnt_phi;
    factor_distinct(phi, fac_phi, &cnt_phi);

    /* Prime powers of p+1 */
    uint64_t fac_pp[64];
    int exp_pp[64];
    int cnt_pp;
    factor_with_exp(p + 1, fac_pp, exp_pp, &cnt_pp);

    /* Generate all divisors of p+1. */
    uint64_t divs[4096];
    size_t divcnt = 1;
    divs[0] = 1;
    for (int i = 0; i < cnt_pp; ++i) {
        uint64_t base = 1;
        size_t old_cnt = divcnt;
        for (int e = 1; e <= exp_pp[i]; ++e) {
            base *= fac_pp[i];
            for (size_t j = 0; j < old_cnt; ++j)
                divs[divcnt++] = divs[j] * base;
        }
    }

    /* Check each divisor d in (1, p). */
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d <= 1 || d >= p) continue;
        bool primitive = true;
        for (int j = 0; j < cnt_phi; ++j) {
            uint64_t q = fac_phi[j];
            if (mod_pow((seq_t)d, phi / q, (seq_t)p) == (seq_t)1) {
                primitive = false;
                break;
            }
        }
        if (primitive) return true;            /* found a divisor that is a primitive root */
    }
    return false;                              /* none found */
}

/* ---------------------------------------------------------------------- */
/* Memoisation of the sequence ---------------------------------------- */
/* ---------------------------------------------------------------------- */
static seq_t  *memo    = NULL;    /* array of terms */
static size_t  memo_cap = 0;     /* allocated capacity */
static size_t  memo_len = 0;     /* number of terms stored */

static void ensure_memo_cap(size_t need)
{
    if (need <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memo = tmp;
    memo_cap = new_cap;
}

static void memo_add(uint64_t val)
{
    ensure_memo_cap(memo_len + 1);
    memo[memo_len++] = (seq_t)val;
}

/* ---------------------------------------------------------------------- */
/* Main function                                                        */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */

    /* Initialise static data on first call. */
    if (!prime_list) prime_list_init();

    /* Generate terms until we have at least n of them. */
    while (memo_len < (size_t)n) {
        uint64_t p = next_prime(last_checked_prime);
        if (p <= 3) continue;          /* 2 and 3 are not members */
        if (!has_primitive_root_dividing(p))
            memo_add(p);
    }
    return memo[(size_t)n - 1];
}
