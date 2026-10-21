/*
 * A224902  O.g.f.: exp( Sum_{n>=1} (sigma(2*n^4) - sigma(n^4)) * x^n/n ).
 * https://oeis.org/A224902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Sum_{k>=1} (sigma(2*k^4) - sigma(k^4)) * x^k / k )
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-recursive implementation using the recurrence
   a(0) = 1,  a(n) = (1/n) * sum_{k=1}^{n} c_k * a(n-k),
   where  c_k = sigma(2*k^4) - sigma(k^4).  */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment.
   The following utilities are required. */
#include "../utils/seq_pow.h"

/* --- Smallest prime factor table ---------------------------------------- */
static uint32_t *spf = NULL;      /* spf[i] = smallest prime factor of i   */
static size_t   spf_cap = 0;      /* allocated size (>= m+1) */

static void build_spf(idx_t limit)
{
    if ((size_t)limit + 1 <= spf_cap) return;

    size_t new_cap = (size_t)limit + 1;
    uint32_t *old = spf;
    spf = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    if (!spf) { free(old); return; }

    /* initialize */
    for (size_t i = 0; i <= (size_t)limit; ++i) spf[i] = 0;
    if (limit >= 0) spf[0] = 0;
    if (limit >= 1) spf[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = (uint32_t)i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = new_cap;
}

/* --- Compute c_k = sigma(2*k^4) - sigma(k^4) ---------------------------- */
static seq_t compute_c_k(idx_t k)
{
    /* Factor k once; build sigma(k^4) and count exponent of 2. */
    seq_t sigma = (seq_t)1;
    uint32_t t = (uint32_t)k;
    int e2 = 0;

    while (t > 1) {
        uint32_t p = spf[t];
        int e = 0;
        while (t % p == 0) { t /= p; ++e; }
        if (p == 2) e2 = e;                 /* power of 2 in k */

        /* term for sigma(k^4) for prime p : (p^(4e+1) - 1)/(p-1) */
        idx_t exp = 4 * e + 1;
        seq_t term = (seq_pow((seq_t)p, exp) - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= term;
    }

    /* sigma(2*k^4) = sigma * (2^(4e2+2)-1)/(2^(4e2+1)-1)   */
    idx_t exp_num = 4 * e2 + 2;
    idx_t exp_den = 4 * e2 + 1;
    seq_t num = seq_pow((seq_t)2, exp_num) - (seq_t)1;
    seq_t den = seq_pow((seq_t)2, exp_den) - (seq_t)1;
    seq_t sigma2 = sigma * num / den;

    return sigma2 - sigma;
}

/* --- Caches for a(n) and c(n) ----------------------------------------- */
static seq_t *a_cache = NULL;   /* a_cache[0] .. a_cache[max_n]   */
static seq_t *c_cache = NULL;   /* c_cache[0] unused, c_cache[1..] */
static idx_t  cache_cap = 0;    /* number of elements currently stored   */
static idx_t  max_computed = 0; /* highest n for which a_cache[n] is known */

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0)=1 */

    /* Ensure sufficient prime factor table */
    build_spf(n);

    /* Ensure cache size */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        c_cache = (seq_t *)realloc(c_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise a_cache[0] if first use */
    if (max_computed == 0) a_cache[0] = (seq_t)1;

    /* Compute new c(k) values */
    for (idx_t k = max_computed + 1; k <= n; ++k)
        c_cache[k] = compute_c_k(k);

    /* Compute new a(i) values */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)
            sum += c_cache[k] * a_cache[i - k];
        a_cache[i] = sum / i;     /* division is exact */
    }

    max_computed = n;
    return a_cache[n];
}
