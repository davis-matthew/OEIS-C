/*
 * A159309  L.g.f.: Sum_{n>=1} a(n)*x^n/n = Sum_{n>=1} (1 + sigma(n)*x)^n *
 *          x^n/n.
 * https://oeis.org/A159309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k=0..⌊n/2⌋} binom(n-k, k) * sigma(n-k)^k / (n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom_n_k, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_n_k.h"
#include "../utils/seq_pow.h"

/* Prime factorization helper: smallest prime factor array */
static idx_t *spf_arr = NULL;   /* SPF[0..max] */
static idx_t spf_cap = 0;

/* Sigma (sum‑of‑divisors) cache */
static seq_t *sigma_arr = NULL;  /* sigma_arr[i] = sigma(i) for i <= max */
static idx_t sigma_cap = 0;

/* Ensure that SPF and sigma are computed up to at least n */
static void ensure_cache(idx_t n) {
    if (n < sigma_cap) return;          /* already available */

    idx_t new_cap = n + 1;

    /* --- Prime factorization table --- */
    if (!spf_arr) {
        spf_arr = (idx_t *)calloc(new_cap, sizeof(idx_t));
        for (idx_t i = 2; i < new_cap; ++i) spf_arr[i] = i;
        for (idx_t i = 2; i * i < new_cap; ++i)
            if (spf_arr[i] == i)
                for (idx_t j = i * i; j < new_cap; j += i)
                    if (spf_arr[j] == j) spf_arr[j] = i;
    } else if (spf_cap < new_cap) {
        spf_arr = (idx_t *)realloc(spf_arr, new_cap * sizeof(idx_t));
        for (idx_t i = spf_cap; i < new_cap; ++i) spf_arr[i] = i;
        for (idx_t i = 2; i * i < new_cap; ++i)
            if (spf_arr[i] == i)
                for (idx_t j = i * i; j < new_cap; j += i)
                    if (spf_arr[j] == j) spf_arr[j] = i;
    }
    spf_cap = new_cap;

    /* --- Sigma cache --- */
    if (!sigma_arr) {
        sigma_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        sigma_arr[0] = 0;
        sigma_arr[1] = 1;
    } else if (sigma_cap < new_cap) {
        sigma_arr = (seq_t *)realloc(sigma_arr, new_cap * sizeof(seq_t));
    }

    for (idx_t i = sigma_cap; i < new_cap; ++i) {
        idx_t tmp = i;
        seq_t prod = 1;
        while (tmp > 1) {
            idx_t p = spf_arr[tmp];
            idx_t exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }

            seq_t term = seq_pow((seq_t)p, exp + 1) - 1;   /* p^(e+1)-1 */
            seq_t denom = ((seq_t)p - 1);                 /* p-1  */
            term = term / denom;                          /* exact */
            prod = prod * term;
        }
        sigma_arr[i] = prod;
    }
    sigma_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety, OEIS offset starts at 1 */

    ensure_cache(n);

    seq_t result = (seq_t)0;
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - k;                      /* m = n-k  */
        seq_t sigma_m = sigma_arr[m];
        seq_t binom   = binom_n_k(m, k);       /* C(m,k)  */
        seq_t pow_sigma = seq_pow(sigma_m, k); /* sigma_m^k */
        seq_t term = binom * pow_sigma;
        term = term * n_seq;
        term = term / (seq_t)m;               /* exact division */
        result += term;
    }
    return result;
}
