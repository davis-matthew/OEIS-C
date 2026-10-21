/*
 * A207710  Expansion of f(x) * f(-x^10) / f(-x^2, -x^8) in powers of x where
 *          f() is Ramanujan's two-variable theta function.
 * https://oeis.org/A207710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207710
   G.f.: Prod_{k>0} (1 - (-x)^k) / ((1 - x^(10*k - 2)) * (1 - x^(10*k - 8)))
   The implementation computes all coefficients up to the largest n
   requested and caches them.  Each coefficient is obtained by
   expanding the numerator product (∏(1 - (-1)^k x^k)) first,
   then multiplying by all denominator inverses (∏ 1/(1 - x^m))
   where m ≡ 2 or 8 (mod 10).  The algorithm uses only integer
   arithmetic of type seq_t, performs truncation to degree n,
   and avoids any floating‑point or external libraries.
   Complexity: O(n²) for a new maximum n, which is well below the
   required 2 s limit for n ≤ 10 000. */

#include <stdlib.h>

static seq_t *coeff_cache = NULL;   /* cached coefficients */
static idx_t   cache_len  = 0;      /* cache size (number of elements) */

/* Recompute all coefficients up to max_n (inclusive) and store in cache. */
static void recompute(idx_t max_n)
{
    /* Allocate new cache if needed */
    if (coeff_cache != NULL) {
        free(coeff_cache);
    }
    coeff_cache = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
    if (coeff_cache == NULL) {
        /* In a real build this should be handled, but we assume sufficient
           memory as per problem constraints. */
        exit(1);
    }
    cache_len = max_n + 1;

    /* Initialise to zero, then set coefficient 0 = 1 */
    for (idx_t i = 0; i <= max_n; ++i) {
        coeff_cache[i] = 0;
    }
    coeff_cache[0] = 1;

    /* Numerator product: ∏ (1 - (-1)^k x^k) */
    for (idx_t k = 1; k <= max_n; ++k) {
        seq_t s = (k & 1) ? (seq_t)-1 : (seq_t)1;   /* s = (-1)^k */
        for (idx_t j = max_n; j >= k; --j) {
            coeff_cache[j] -= s * coeff_cache[j - k];
        }
    }

    /* Denominator inverses: 1/(1 - x^m) for m ≡ 2 or 8 (mod 10) */
    /* Start with m = 2, 8 and add 10 each step */
    for (idx_t m = 2; m <= max_n; m += 10) {
        for (idx_t j = m; j <= max_n; ++j) {
            coeff_cache[j] += coeff_cache[j - m];
        }
    }
    for (idx_t m = 8; m <= max_n; m += 10) {
        for (idx_t j = m; j <= max_n; ++j) {
            coeff_cache[j] += coeff_cache[j - m];
        }
    }
}

/* Public accessor for a(n) */
seq_t a(idx_t n)
{
    if (n + 1 > cache_len) {
        recompute(n);
    }
    return coeff_cache[n];
}
