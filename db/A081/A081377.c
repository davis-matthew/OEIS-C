/*
 * A081377  Numbers k such that the set of prime divisors of phi(k) is equal
 *          to the set of prime divisors of sigma(k).
 * https://oeis.org/A081377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k (n≥1) such that the set of distinct
 *            prime divisors of φ(k) equals that of σ(k)
 * Range:     u8 n <= 12, u16 n <= 303, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   same_prime_sets
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/same_prime_sets.h"

/* ------------------------------------------------------------ */
/*  Prime table – fixed limit large enough for the required range */
#define PRIME_LIMIT   1000000        /* enough for k up to about 1e12 */
static uint32_t primes[PRIME_LIMIT + 1];   /* primes[0…prime_cnt-1] */
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* ------------------------------------------------------------ */
/*  Simple sieve to fill the primes array                     */
static void init_primes(void)
{
    if (primes_inited) return;
    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, 1);
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = true;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            primes[prime_cnt++] = (uint32_t)i;
    free(is_composite);
    primes_inited = true;
}

/* ------------------------------------------------------------ */
/*  Factorisation helper – collects distinct prime factors      */
static void factor_distinct(uint64_t n, uint32_t *factors, size_t *cnt)
{
    size_t c = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            factors[c++] = p;
            do { n /= p; } while (n % p == 0);
        }
    }
    if (n > 1) factors[c++] = (uint32_t)n;   /* remaining prime */
    *cnt = c;
}

/* ------------------------------------------------------------ */
/*  Fast power with 128‑bit intermediate to avoid overflow     */
static uint64_t pow_u64(uint64_t base, unsigned exp)
{
    __uint128_t res = 1;
    __uint128_t b = base;
    while (exp) {
        if (exp & 1) res *= b;
        b *= b;
        exp >>= 1;
    }
    return (uint64_t)res;   /* assumes result fits in 64 bits */
}

/* ------------------------------------------------------------ */
/*  Compute φ(k) and σ(k) from the factorisation of k; also
   return the list of distinct primes of k (used for later steps). */
static void phi_sigma_from_factors(uint64_t k,
                                   uint64_t *phi, uint64_t *sigma,
                                   uint32_t *pf, uint32_t *exp, size_t *cnt)
{
    uint64_t phi_val = 1;
    uint64_t sigma_val = 1;
    size_t c = 0;
    uint64_t n = k;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[c]   = p;
            exp[c]  = (uint32_t)e;
            ++c;

            /* φ part: p^(e-1)*(p-1) */
            uint64_t part_phi = pow_u64(p, e - 1) * (p - 1);
            phi_val *= part_phi;

            /* σ part: (p^(e+1)-1)/(p-1) */
            uint64_t part_sigma = (pow_u64(p, e + 1) - 1) / (p - 1);
            sigma_val *= part_sigma;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        pf[c]  = (uint32_t)n;
        exp[c] = 1;
        ++c;
        uint64_t p = n;
        phi_val *= (p - 1);
        uint64_t part_sigma = (p * p - 1) / (p - 1);  /* σ(p)=1+p */
        sigma_val *= part_sigma;
    }
    *cnt = c;
    *phi = phi_val;
    *sigma = sigma_val;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*  Storage for the sequence values                             */
static seq_t *seq_vals      = NULL;
static size_t seq_len       = 0;
static size_t seq_cap       = 0;

/* ------------------------------------------------------------ */
/*  The main function – returns the n-th term (n≥1)             */
seq_t a(idx_t n)
{
    /* Initialise static data on first call */
    if (seq_len == 0) {
        init_primes();                     /* fill the prime table */

        seq_cap = 64;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_vals) exit(1);

        seq_len = 0;
    }

    /* Generate terms until we have at least n of them */
    static uint64_t current_k = 1;          /* next candidate integer to test */

    while (seq_len < n) {
        uint64_t k = current_k++;

        /* Factorise k to compute φ(k) and σ(k) */
        uint64_t phi, sigma;
        uint32_t pf[64], exp[64];
        size_t cnt = 0;
        phi_sigma_from_factors(k, &phi, &sigma, pf, exp, &cnt);

        /* Factorise φ(k) and σ(k) to obtain their prime‑divisor sets */
        uint32_t primes_phi[64], primes_sigma[64];
        size_t cnt_phi, cnt_sigma;
        factor_distinct(phi, primes_phi, &cnt_phi);
        factor_distinct(sigma, primes_sigma, &cnt_sigma);

        /* Compare the two sets */
        if (same_prime_sets(primes_phi, cnt_phi, primes_sigma, cnt_sigma)) {
            /* Ensure capacity */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
                if (!seq_vals) exit(1);
            }
            seq_vals[seq_len++] = (seq_t)k;   /* cast fits because k is uint64_t */
        }
    }

    return seq_vals[n - 1];
}
