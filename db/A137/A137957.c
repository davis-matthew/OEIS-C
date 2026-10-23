/*
 * A137957  G.f. satisfies A(x) = 1 + x*(1 + x*A(x)^4)^3.
 * https://oeis.org/A137957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A137957
   Formula: a(0) = 1, a(n) = sum_{k=0}^{n-1} C(3*(n-k),k)/(n-k) * C(4*k,n-k-1) for n>0
   (C is the binomial coefficient)
   The code memoises computed values to be fast for arbitrary call order.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Multiplicative binomial coefficient: C(n,k) using seq_t arithmetic  */
/* --------------------------------------------------------------------- */
static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division for binomial */
    }
    return res;
}

/* --------------------------------------------------------------------- */
/*  Compute a(n) from the closed‑form sum (no memoisation)          */
/* --------------------------------------------------------------------- */
static seq_t compute_a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        idx_t nmk = n - k;                     /* n-k > 0 for k<n */
        /* C(3*(n-k), k) */
        idx_t m1 = 3U * nmk;
        seq_t c1 = binom_u64(m1, k);
        /* C(4*k, n-k-1) */
        idx_t m2 = 4U * k;
        idx_t r2 = nmk - 1U;                   /* n-k-1 >= 0 */
        seq_t c2 = binom_u64(m2, r2);
        seq_t term = c1 * c2;
        term = term / (seq_t)nmk;              /* division by (n-k) is exact */
        sum += term;
    }
    return sum;
}

/* --------------------------------------------------------------------- */
/*  Memoised version of a(n)                                              */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cached_max = 0;

/* Ensure cache[0..n] is allocated and contains a(0)..a(n) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* first allocation must contain at least one element for a(0) */
        size_t sz = (size_t)((n > 0) ? n + 1 : 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return;          /* allocation failure – caller will handle */
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)1;
        cached_max = (n >= 1) ? 1 : 0;
    } else if (n > cached_max) {
        size_t sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
        if (!tmp) return;            /* allocation failure – caller will handle */
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cached_max + 1; i <= n; ++i)
            cache[i] = compute_a(i);
        cached_max = n;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache) return cache[n];
    /* If we reach this point allocation failed; return 0 as a fallback. */
    return (seq_t)0;
}
