/*
 * A067268  Numbers k such that k and k^2+1 have the same number of distinct
 *          prime factors.
 * https://oeis.org/A067268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Search integers k and record those with omega(k) = omega(k^2+1)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* limit for the smallest–prime‑factor sieve (≈40 MB for build‑time memory) */
#ifndef SPF_LIMIT
#define SPF_LIMIT 10000000u
#endif

/*--- Prime‑factor sieve -------------------------------------------*/
static uint32_t *spf     = NULL;   /* smallest prime factor table */
static uint32_t *prime_l = NULL;   /* list of primes in ascending order */
static size_t    pcnt    = 0;      /* number of primes stored */

static void build_sieve(void)
{
    spf = (uint32_t*)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    memset(spf, 0, (SPF_LIMIT + 1) * sizeof(uint32_t));

    /* rough upper bound for the number of primes up to SPF_LIMIT */
    prime_l = (uint32_t*)malloc((SPF_LIMIT / 10) * sizeof(uint32_t));
    if (!prime_l) exit(1);

    for (uint32_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {                   /* i is prime */
            spf[i] = i;
            prime_l[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt &&
                            prime_l[j] <= spf[i] &&
                            (uint64_t)i * prime_l[j] <= SPF_LIMIT;
             ++j)
            spf[i * prime_l[j]] = prime_l[j];
    }
    /* shrink to the exact size */
    prime_l = (uint32_t*)realloc(prime_l, pcnt * sizeof(uint32_t));
}

/*--- Number of distinct prime factors (omega) ---------------------*/
static inline size_t omega_small(uint32_t x)
{
    size_t c = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        ++c;
        while (x % p == 0) x /= p;
    }
    return c;
}

static inline size_t omega_large(uint64_t n)
{
    size_t c = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p   = prime_l[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) {
            ++c;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) ++c;      /* remaining prime factor > sqrt(original n) */
    return c;
}

/*--- Storage for the generated sequence --------------------------*/
static seq_t *seq_vals = NULL;
static size_t seq_cap  = 0;
static size_t seq_cnt  = 0;

/*--- Current integer that is being examined ---------------------*/
static idx_t k_current = 2;
static bool   init_done = false;

static inline void ensure_init(void)
{
    if (!init_done) {
        build_sieve();
        init_done = true;
    }
}

static inline void push_k(uint64_t k)
{
    if (seq_cnt >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_cnt++] = (seq_t)k;
}

/*------------------------------------------------------------------------*/
/*  Public function required by the test harness                         */
/*------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    ensure_init();

    while (seq_cnt < (size_t)n) {
        uint64_t k = k_current;
        size_t   omega_k;
        if (k <= SPF_LIMIT)
            omega_k = omega_small((uint32_t)k);
        else
            omega_k = omega_large(k);

        uint64_t k2p1 = k * k + 1ULL;
        size_t   omega_k2 = omega_large(k2p1);

        if (omega_k == omega_k2)
            push_k(k);

        ++k_current;
    }
    return seq_vals[n - 1];      /* offset 1: a(1) is the first term */
}
