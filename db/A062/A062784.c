/*
 * A062784  Numbers k such that sigma(k) + phi(k) is a perfect square.
 * https://oeis.org/A062784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062784: Numbers k such that σ(k)+φ(k) is a perfect square. */
/* The function generates terms by sieving k for increasing values until the
 * requested index n has been found.  It uses a linear sieve to retrieve
 * prime factorizations efficiently. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ----- Sieve data ----------------------------------------------------- */
static uint32_t *spf = NULL;   /* smallest prime factor for each integer */
static size_t spf_cap = 0;     /* current limit of the sieve (inclusive) */
static uint32_t *primes = NULL;/* list of primes found */
static size_t prime_cnt = 0;   /* number of primes */

/* ----- Sequence storage ---------------------------------------------- */
#define MAX_SEQUENCE 20000   /* maximum number of terms we expect */
static seq_t seq_vals[MAX_SEQUENCE];
static size_t seq_len = 0;
static uint64_t last_k = 0;  /* largest k already examined */

/* ----- Forward declarations ------------------------------------------- */
static void init_sieve(void);
static void extend_sieve(uint64_t new_cap);
static void sigma_phi(uint64_t k, seq_t *sigma_out, seq_t *phi_out);

/* ----- Initialise the sieve up to a given bound ----------------------- */
static void init_sieve(void)
{
    if (spf_cap != 0) return;  /* already initialised */

    /* Initial capacity – enough for the first few thousand terms. */
    const size_t INITIAL_CAP = 2000000;  /* 2 × 10⁶ */
    spf_cap = INITIAL_CAP;
    spf = (uint32_t *)calloc(spf_cap + 1, sizeof(uint32_t));

    /* Allocate space for primes; the number of primes below n ≈ n / ln n. */
    size_t prime_cap = spf_cap / 10 + 100;
    primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));

    /* Linear sieve algorithm. */
    for (size_t i = 2; i <= spf_cap; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > spf_cap) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
}

/* ----- Extend the sieve when we need a larger bound ------------------ */
static void extend_sieve(uint64_t new_cap)
{
    if (new_cap <= spf_cap) return;

    /* Ensure new_cap is at least twice the current cap for amortised cost. */
    while (spf_cap < new_cap) {
        spf_cap <<= 1;
    }

    /* Resize spf array. */
    spf = (uint32_t *)realloc(spf, (spf_cap + 1) * sizeof(uint32_t));
    memset(spf + new_cap + 1, 0, (spf_cap - new_cap) * sizeof(uint32_t));

    /* Resize primes array if necessary. */
    size_t prime_cap = prime_cnt + spf_cap / 10 + 10;
    primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));

    /* Linear sieve for the new range. */
    for (size_t i = new_cap + 1; i <= spf_cap; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > spf_cap) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
}

/* ----- Compute σ(k) and φ(k) using the pre‑computed spf table --------- */
static void sigma_phi(uint64_t k, seq_t *sigma_out, seq_t *phi_out)
{
    seq_t sigma = 1;
    seq_t phi = 1;
    uint64_t t = k;
    while (t > 1) {
        uint32_t p = spf[t];
        size_t exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }

        /* σ factor: 1 + p + p² + … + p^exp */
        seq_t sigma_factor = 1;
        seq_t power = 1;
        for (size_t i = 0; i < exp; ++i) {
            power *= (seq_t)p;
            sigma_factor += power;
        }
        sigma *= sigma_factor;

        /* φ factor: p^(exp‑1) * (p‑1) */
        seq_t phi_factor;
        if (exp == 1) {
            phi_factor = (seq_t)(p - 1);
        } else {
            seq_t p_power = 1;
            for (size_t i = 0; i < exp - 1; ++i)
                p_power *= (seq_t)p;
            phi_factor = p_power * (seq_t)(p - 1);
        }
        phi *= phi_factor;
    }
    *sigma_out = sigma;
    *phi_out = phi;
}

/* ----- Main function requested by the build system ------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, so n ≥ 1 */

    init_sieve();

    /* Generate terms until we have at least n of them. */
    uint64_t k = last_k + 1;
    while (seq_len < (size_t)n) {
        /* If we are about to exceed the sieve, extend it. */
        if (k > spf_cap) {
            extend_sieve(k + 1);
        }

        seq_t sigma, phi;
        sigma_phi(k, &sigma, &phi);
        seq_t sum = sigma + phi;
        seq_t sq = floor_sqrt(sum);
        if (sq * sq == sum) {
            /* Store the term if we haven't exceeded the pre‑allocated array. */
            if (seq_len < MAX_SEQUENCE) {
                seq_vals[seq_len++] = (seq_t)k;
            }
        }
        ++k;
    }
    last_k = k - 1;

    return seq_vals[n - 1];
}
