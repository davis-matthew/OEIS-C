/*
 * A055464  Numbers n such that sum of EulerPhi and DivisorSum is an integer
 *          multiple of the number of divisors.
 * https://oeis.org/A055464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_sigma_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) = n-th positive integer k such that (φ(k)+σ(k)) mod τ(k) == 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/phi_sigma_tau.h"

/* Precompute smallest prime factor for numbers up to LIMIT. */
static uint32_t *spf = NULL;
static size_t spf_limit = 0;

static void init_spf(size_t limit)
{
    if (limit <= spf_limit) return;        /* already initialized */
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* Compute φ(k)+σ(k) and τ(k) using spf. */
static void compute_phi_sigma_tau(idx_t k, seq_t *sum_out, uint64_t *tau_out)
{
    seq_t phi = 1;
    seq_t sigma = 1;
    uint64_t tau = 1;
    idx_t n = k;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }

        /* compute p^e */
        seq_t p_pow = 1;
        for (int i = 0; i < e; ++i) p_pow *= p;

        /* φ component: p^e - p^(e-1) = p^(e-1)(p-1) */
        seq_t phi_comp = p_pow - p_pow / p;

        /* σ component: 1 + p + p^2 + ... + p^e */
        seq_t sigma_comp = 1;
        seq_t term = 1;
        for (int i = 1; i <= e; ++i) {
            term *= p;
            sigma_comp += term;
        }

        phi *= phi_comp;
        sigma *= sigma_comp;
        tau *= (uint64_t)(e + 1);
    }
    *sum_out = phi + sigma;
    *tau_out = tau;
}

/* Main sequence generator with memoization. */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t next_candidate = 1;
static const size_t INITIAL_LIMIT = 200000;

static void ensure_terms(idx_t up_to)
{
    while (terms_len < up_to) {
        /* ensure spf covers next_candidate */
        if ((size_t)next_candidate > spf_limit) {
            init_spf((size_t)next_candidate * 2);
        }
        seq_t sum;
        uint64_t tau;
        compute_phi_sigma_tau(next_candidate, &sum, &tau);
        if (sum % tau == 0) {
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap == 0 ? 128 : terms_cap * 2;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!terms) exit(1);
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n < 1) return 0;          /* undefined for n==0, just return 0 */
    if (!terms) {                            /* first call: init */
        init_spf(INITIAL_LIMIT);
        terms = NULL;
        terms_cap = 0;
        terms_len = 0;
        next_candidate = 1;
    }
    ensure_terms(n);
    return terms[n - 1];
}
