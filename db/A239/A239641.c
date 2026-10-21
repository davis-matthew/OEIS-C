/*
 * A239641  Dirichlet inverse of Carmichael lambda function (A002322).
 * https://oeis.org/A239641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the Dirichlet inverse of the Carmichael lambda function
 *            λ(n) (OEIS A002322). The function a(n) satisfies ∑_{d|n} λ(d)
 *            a(n/d) = ε(n), with ε(1)=1 and ε(n)=0 for n>1
 * Range:     i8 n <= 90, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"

static seq_t *cache_g = NULL;
static bool *seen_g  = NULL;
static size_t cap_g  = 0;

static seq_t *cache_lam = NULL;
static bool *seen_lam  = NULL;
static size_t cap_lam  = 0;

/* Expand cache arrays to accommodate indices up to n. */
static void ensure_cache(idx_t n, seq_t **cache, bool **seen, size_t *cap) {
    if ((size_t)n + 1 <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
    *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    *seen  = (bool  *)realloc(*seen , new_cap * sizeof(bool));
    for (size_t i = *cap; i < new_cap; ++i) (*seen)[i] = false;
    *cap = new_cap;
}

/* Carmichael lambda function λ(m).  Results are cached. */
static seq_t lambda_of(idx_t m) {
    if (m == 1) return (seq_t)1;
    ensure_cache(m, &cache_lam, &seen_lam, &cap_lam);
    if (seen_lam[m]) return cache_lam[m];

    seq_t res = 1;
    idx_t temp = m;

    /* factor out powers of 2 */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t lam2;
        if (e == 1)        lam2 = 1;
        else if (e == 2)   lam2 = 2;
        else {
            lam2 = 1;
            for (idx_t i = 0; i < e - 2; ++i) lam2 <<= 1;  /* 2^(e-2) */
        }
        seq_t g = seq_gcd(res, lam2);
        res = (res / g) * lam2;
    }

    /* factor odd primes */
    for (idx_t p = 3; (idx_t)(p * p) <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t lam = seq_pow((seq_t)p, e - 1);   /* p^(e-1) */
            lam = lam * (seq_t)(p - 1);             /* p^(e-1)*(p-1) */
            seq_t g = seq_gcd(res, lam);
            res = (res / g) * lam;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        seq_t lam = (seq_t)(temp - 1);  /* e=1 => just p-1 */
        seq_t g = seq_gcd(res, lam);
        res = (res / g) * lam;
    }

    cache_lam[m] = res;
    seen_lam[m]  = true;
    return res;
}

/* Main sequence function: Dirichlet inverse of λ(n). */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    ensure_cache(n, &cache_g, &seen_g, &cap_g);
    if (seen_g[n]) return cache_g[n];

    seq_t sum = 0;
    for (idx_t d = 2; d <= n; ++d) {
        if (n % d == 0) {
            seq_t lam  = lambda_of(d);
            seq_t term = a(n / d);
            sum += lam * term;
        }
    }
    seq_t result = -sum;

    cache_g[n] = result;
    seen_g[n]  = true;
    return result;
}
