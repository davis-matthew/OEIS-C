/*
 * A064062  Generalized Catalan numbers C(2; n).
 * https://oeis.org/A064062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A064062: Generalized Catalan numbers C(2; n)
   Recurrence: a(0) = 1,
               a(n) = Σ_{k=0}^{n-1} Catalan(k) * 2^k * a(n-1-k)
   (coefficients of C(2x) convolved with a).  All values are exact
   for the type seq_t when it fits; otherwise use arbitrary‑precision. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/catalan.h"   /* seq_t catalan(idx_t k) */
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp) */

/* Cache for computed terms and auxiliary values. */
/* a_cache[n]  = a(n)
   cat_cache[n] = Catalan(n)
   pow_cache[n] = 2^n
   cache_cap   = current capacity of all three arrays
   max_done    = largest index for which a_cache is valid   */
static seq_t *a_cache   = NULL;
static seq_t *cat_cache = NULL;
static seq_t *pow_cache = NULL;
static size_t cache_cap = 0;
static idx_t max_done = (idx_t)-1;   /* none computed yet */

/* Ensure that the cache arrays can hold indices up to 'need-1'.
   New capacity is doubled until it reaches 'need'.  Existing
   entries are copied to the new arrays.  This routine is safe for
   both C and C++ (big‐integer types are copied element‑wise). */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_a   = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_cat = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_pow = (seq_t*)malloc(new_cap * sizeof(seq_t));

    if (a_cache) {
        for (size_t j = 0; j < cache_cap; ++j) new_a[j] = a_cache[j];
        free(a_cache);
    }
    if (cat_cache) {
        for (size_t j = 0; j < cache_cap; ++j) new_cat[j] = cat_cache[j];
        free(cat_cache);
    }
    if (pow_cache) {
        for (size_t j = 0; j < cache_cap; ++j) new_pow[j] = pow_cache[j];
        free(pow_cache);
    }

    a_cache   = new_a;
    cat_cache = new_cat;
    pow_cache = new_pow;
    cache_cap = new_cap;
}

/* Main sequence accessor.  Computes a(n) on demand and memoizes
   all intermediate values.  The function is thread‑unsafe but
   satisfies the requested behaviour for single‑threaded
   evaluations. */
seq_t a(idx_t n)
{
    /* Make sure we have storage for a(0)..a(n). */
    ensure_capacity((size_t)n + 1);

    /* Compute values that are missing. */
    idx_t start = (max_done == (idx_t)-1) ? 0 : max_done + 1;
    for (idx_t i = start; i <= n; ++i) {
        /* Precompute auxiliary values for use in later terms. */
        cat_cache[i] = catalan(i);
        pow_cache[i] = pow2(i);

        if (i == 0) {
            a_cache[0] = (seq_t)1;           /* a(0) = 1 */
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t term = cat_cache[k];
                term *= pow_cache[k];
                term *= a_cache[i - 1 - k];
                sum += term;
            }
            a_cache[i] = sum;
        }
        max_done = i;
    }
    return a_cache[n];
}
