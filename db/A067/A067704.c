/*
 * A067704  Numbers n such that phi(sigma(n)) = 2*phi(n).
 * https://oeis.org/A067704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the numbers n such that φ(σ(n)) = 2·φ(n) Method: compute
 *            φ(n) and σ(n) by a linear sieve up to a fixed LIMIT, then select
 *            n that satisfy φ(σ(n)) == 2*φ(n)
 * Range:     u8 n <= 12, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static const size_t LIMIT = 5000000;   /* covers many more terms than needed */

static void add_term(seq_t value)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1;
        seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        seq_vals = new_arr;
        seq_cap = new_cap;
    }
    seq_vals[seq_len++] = value;
}

static void precompute(void)
{
    /* Linear sieve for smallest prime factor */
    uint32_t *spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *phi = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    uint64_t *sigma     = (uint64_t *)calloc(LIMIT + 1, sizeof(uint64_t));
    uint32_t *sigma_pow = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *primes   = (uint32_t *)malloc((LIMIT / 10 + 100) * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t val = (uint64_t)i * p;
            if (p > spf[i] || val > LIMIT) break;
            spf[val] = p;
        }
    }

    phi[1] = 1;
    sigma[1] = 1;
    sigma_pow[1] = 1;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        uint32_t p = spf[i];
        uint32_t j = i / p;
        if (j % p == 0) {
            sigma_pow[i] = sigma_pow[j] * p + 1;
            sigma[i] = sigma[j] / sigma_pow[j] * sigma_pow[i];
            phi[i] = phi[j] * p;
        } else {
            sigma_pow[i] = p + 1;
            sigma[i] = sigma[j] * sigma_pow[i];
            phi[i] = phi[j] * (p - 1);
        }
    }

    for (uint32_t n = 2; n <= LIMIT; ++n) {
        uint64_t sigma_val = sigma[n];
        uint64_t x = sigma_val;
        uint64_t phi_sigma = sigma_val;
        for (size_t k = 0; k < primes_cnt && (uint64_t)primes[k] * primes[k] <= x; ++k) {
            uint32_t pr = primes[k];
            if (x % pr == 0) {
                phi_sigma -= phi_sigma / pr;
                while (x % pr == 0) x /= pr;
            }
        }
        if (x > 1) phi_sigma -= phi_sigma / x;

        if (phi_sigma == (uint64_t)2 * phi[n]) {
            add_term((seq_t)n);
        }
    }

    free(spf);
    free(phi);
    free(sigma);
    free(sigma_pow);
    free(primes);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (seq_vals == NULL) {
        precompute();
    }
    if ((idx_t)(n - 1) < seq_len) {
        return seq_vals[n - 1];
    }
    return (seq_t)0;
}
