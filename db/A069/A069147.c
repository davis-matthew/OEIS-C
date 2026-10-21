/*
 * A069147  Numbers k such that tau_3(k) (the number of ordered factorizations
 *          of k as k = r*s*t) divides k.
 * https://oeis.org/A069147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069147: k such that tau_3(k) divides k (ordered factorizations). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global storage for the computed terms */
static seq_t *seq_vals = NULL;
static idx_t seq_len  = 0;
static bool initialized = false;

/* Generate the sequence up to a fixed limit.  The limit is chosen
   to comfortably contain the first 10000 terms.  It can be increased
   if needed but the current values work for typical test sizes. */
static void generate_sequence(void)
{
    const uint32_t LIMIT = 6000000U;          /* 6 000 000 */
    uint32_t *spf      = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    uint16_t *exp_spf  = (uint16_t*)calloc(LIMIT + 1, sizeof(uint16_t));
    uint32_t *tau      = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *tau3     = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *primes   = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
    size_t primes_cnt  = 0;

    if (!spf || !exp_spf || !tau || !tau3 || !primes) abort();

    /* Linear sieve for smallest prime factor */
    spf[1] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > spf[i] || prod > LIMIT) break;
            spf[(uint32_t)prod] = p;
        }
    }

    /* Compute divisor count tau[n] using spf */
    tau[1] = 1;
    exp_spf[1] = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        uint32_t p = spf[i];
        uint32_t m = i / p;
        if (spf[m] == p) {
            exp_spf[i] = exp_spf[m] + 1;
            tau[i] = (uint32_t)(((uint64_t)tau[m] / (exp_spf[m] + 1)) *
                                (exp_spf[i] + 1));
        } else {
            exp_spf[i] = 1;
            tau[i] = tau[m] * 2U;
        }
    }

    /* Compute tau3[n] = sum_{d|n} tau[d] by adding tau[d] to all multiples of d */
    for (uint32_t d = 1; d <= LIMIT; ++d) {
        uint32_t val = tau[d];
        for (uint32_t m = d; m <= LIMIT; m += d) {
            tau3[m] += val;
        }
    }

    /* Collect all k such that k % tau3[k] == 0 */
    size_t cap = 16384;
    seq_vals = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!seq_vals) abort();
    seq_len = 0;

    for (uint32_t k = 1; k <= LIMIT; ++k) {
        if (k % tau3[k] == 0) {
            if (seq_len == cap) {
                cap <<= 1;
                seq_vals = (seq_t*)realloc(seq_vals, cap * sizeof(seq_t));
                if (!seq_vals) abort();
            }
            seq_vals[seq_len++] = (seq_t)k;
        }
    }

    /* Free temporary arrays */
    free(primes);
    free(spf);
    free(exp_spf);
    free(tau);
    free(tau3);
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (!initialized) {
        generate_sequence();
        initialized = true;
    }

    /* OEIS offset is 1 */
    if (n == 0 || n > seq_len) return 0;
    return seq_vals[n - 1];
}
