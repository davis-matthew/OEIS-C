/*
 * A099378  Denominators of the harmonic means of the divisors of the positive
 *          integers.
 * https://oeis.org/A099378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = σ(n) / gcd(n·τ(n), σ(n))
 * Range:     u8 n <= 16, u16 n <= 2436, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of the harmonic means of the divisors of the positive integers.
   a(1) = 1, a(2) = 3, a(3) = 2, a(4) = 7, … */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/gcd.h"
#include "../utils/build_spf.h"
#include "../utils/seq_pow.h"

/* ---------- cached smallest‑prime‑factor array ---------- */
static idx_t *spf      = NULL;        /* spf[i] = smallest prime factor of i, i ≥ 2 */
static idx_t  spf_max  = 0;           /* maximum index currently stored in spf */

/* Ensure spf contains information for all integers up to n */
static void
ensure_spf(idx_t n)
{
    if (n <= spf_max) return;

    /* Free old array, if any, before allocating a new one. */
    if (spf) free(spf);

    build_spf(n, &spf);
    spf_max = n;
}

/* ---------- cached function values ---------- */
static seq_t *cache     = NULL;        /* cache[i] holds a(i); entry 0 unused */
static idx_t  cache_sz  = 0;           /* allocated array length */

/* Ensure cache can hold values up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_sz) return;     /* already large enough */

    /* Reallocate cache to accommodate indices 0…n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();             /* allocation failure – stop safely */

    /* Initialize newly added entries to 0 (sentinel). */
    for (idx_t i = cache_sz; i <= n; ++i) {
        new_cache[i] = 0;
    }

    cache     = new_cache;
    cache_sz  = n + 1;
}

/* ---------- main sequence routine ---------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0 – harmless */

    ensure_cache(n);
    if (cache[n] != 0)                   /* already computed */
        return cache[n];

    ensure_spf(n);

    idx_t m = n;
    seq_t tau   = 1;                     /* τ(n) */
    seq_t sigma = 1;                     /* σ(n) */

    while (m > 1) {
        idx_t p = spf[m];
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        tau *= (seq_t)(e + 1);

        /* σ(p^e) = (p^(e+1) – 1) / (p – 1) */
        seq_t p_pow = seq_pow((seq_t)p, e + 1); /* p^(e+1) */
        seq_t numer = p_pow - (seq_t)1;
        seq_t denom = (seq_t)p - (seq_t)1;
        seq_t part  = numer / denom;

        sigma *= part;
    }

    seq_t N   = (seq_t)n * tau;
    seq_t g   = gcd(N, sigma);
    seq_t denom = sigma / g;

    cache[n] = denom;
    return denom;
}
