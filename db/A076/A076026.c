/*
 * A076026  Expansion of g.f.: (1-4*x*C)/(1-5*x*C) where C =
 *          (1/2-1/2*(1-4*x)^(1/2))/x = g.f. for Catalan numbers A000108.
 * https://oeis.org/A076026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A076026
   Recurrence (in OEIS notation):
   a(0) = 1
   For n >= 1:
       a(n) = 5 * Σ_{k=0..n-1} Catalan(k) * a(n-1-k)  - 4 * Catalan(n-1)
       = 5 * (Catalan(0)*a(n-1) + Catalan(1)*a(n-2) + … + Catalan(n-1)*a(0)) - 4*C_{n-1}
   where Catalan(k) is the k-th Catalan number (A000108).
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

/* ----------  local dynamic‑array helpers  ---------- */
static void ensure_cap(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < needed)
        new_cap <<= 1;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* ----------  memoised sequence  ---------- */
static seq_t *a_cache  = NULL;   /* cache of a(n) */
static size_t a_cap = 0;         /* allocated capacity for a_cache */

static seq_t *c_cache  = NULL;   /* cache of Catalan numbers */
static size_t c_cap = 0;         /* allocated capacity for c_cache */
static idx_t c_max = (idx_t)-1;  /* highest k for which Catalan(k) is cached */

static idx_t a_max = (idx_t)-1;  /* highest n for which a(n) is cached */

/* Helper: make sure Catalan numbers up to k are cached */
static void cache_catalan_upto(idx_t k)
{
    if (k <= c_max)
        return;
    ensure_cap((size_t)(k + 1), &c_cap, &c_cache);
    for (idx_t i = c_max + 1; i <= k; ++i)
        c_cache[i] = catalan(i);
    c_max = k;
}

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    /* Lazily initialise caches at first use */
    if (a_max == (idx_t)-1) {
        ensure_cap(1, &a_cap, &a_cache);
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }
    if (c_max == (idx_t)-1) {
        ensure_cap(1, &c_cap, &c_cache);
        c_cache[0] = (seq_t)catalan(0);
        c_max = 0;
    }

    if (n <= a_max)          /* already computed */
        return a_cache[n];

    /* Ensure space for results up to n */
    ensure_cap((size_t)(n + 1), &a_cap, &a_cache);
    cache_catalan_upto(n - 1);   /* Catalan numbers up to n-1 are needed */

    /* Compute terms a(a_max+1) … a(n) */
    for (idx_t m = a_max + 1; m <= n; ++m) {
        seq_t conv = (seq_t)0;
        /* Σ_{k=0..m-1} Catalan(k) * a(m-1-k) */
        for (idx_t k = 0; k <= m - 1; ++k)
            conv += c_cache[k] * a_cache[m - 1 - k];

        seq_t term1 = conv * (seq_t)5;
        seq_t term2 = c_cache[m - 1] * (seq_t)4;
        a_cache[m] = term1 - term2;
    }
    a_max = n;
    return a_cache[n];
}
