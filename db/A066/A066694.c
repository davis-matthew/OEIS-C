/*
 * A066694  Numbers k such that phi(sigma(k)) > k.
 * https://oeis.org/A066694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function calculates the n‑th term of OEIS A066694:
   the n‑th integer k for which φ(σ(k)) > k.
   It does this by scanning the natural numbers, computing σ(k) and
   φ(σ(k)) using a pre‑computed smallest‑prime‑factor (SPF) sieve,
   and collecting the qualifying k in a static array. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define LIMIT 6000000  /* upper bound that comfortably contains σ(k) */
/* SPF array: spf[x] = smallest prime factor of x   (x >= 2) */
static uint32_t *spf = NULL;
static bool spf_initialized = false;

static void init_spf(void)
{
    if (spf_initialized) return;
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) { exit(1); }
    for (uint32_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    if (!spf[j]) spf[j] = i;
                }
            }
        }
    }
    spf_initialized = true;
}

/* return the sum of divisors σ(n)   (n <= LIMIT) */
static seq_t sigma_uint32(uint32_t n)
{
    seq_t result = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        /* compute (p^(e+1)-1)/(p-1) */
        seq_t pe = 1;
        for (uint32_t i = 0; i <= e; ++i) pe *= p;        /* pe = p^(e+1) */
        seq_t numerator = pe - 1;
        seq_t denominator = p - 1;
        seq_t term = numerator / denominator;            /* exact division */
        result *= term;
    }
    return result;
}

/* return φ(m)   (m <= LIMIT) */
static seq_t phi_uint32(uint32_t m)
{
    seq_t result = m;
    while (m > 1) {
        uint32_t p = spf[m];
        uint32_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        result = result / p * (p - 1); /* equivalent to result *= (1-1/p) */
    }
    return result;
}

static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;
static size_t seq_count = 0;
static uint32_t current_k = 0;

static void ensure_seq_capacity(void)
{
    if (seq_count >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) { exit(1); }
        seq_vals = tmp;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    init_spf();
    while (seq_count < (size_t)n) {
        ++current_k;
        seq_t sig = sigma_uint32(current_k);
        seq_t phi_sig = phi_uint32((uint32_t)sig); /* sig fits in uint32 under LIMIT */
        if (phi_sig > (seq_t)current_k) {
            ensure_seq_capacity();
            seq_vals[seq_count++] = (seq_t)current_k;
        }
    }
    return seq_vals[n - 1];
}
