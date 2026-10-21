/*
 * A053256  Coefficients of the '5th-order' mock theta function f_0(q).
 * https://oeis.org/A053256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 110, i16 n <= 325, i32 n <= 1051, i64 n <= 3665,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A053256: Coefficients of the 5th‑order mock theta function f_0(q).
   The coefficient a(n) is given by
      a(n) = Σ_{k^2 ≤ n} F_k(n − k^2),
   where the sequence F_k(m) is defined recursively by
      F_0(0) = 1,
      F_k(m) = F_{k−1}(m)  –  F_k(m − k)   (m ≥ k).
   This implementation rebuilds a(n) from scratch whenever a larger n is
   requested; its complexity is O(n·√n), easily fast enough for n ≤ 10000.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root (floor) for idx_t. */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= x) {
            if ((mid + 1) > x / (mid + 1)) break;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Cache for the computed sequence. */
static seq_t *a_cache = NULL;      /* array of a(n) */
static idx_t cache_n   = (idx_t)-1; /* highest n for which the cache is valid */

/* Rebuild the cache up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n == cache_n) return;

    size_t sz = (size_t)n + 1;
    seq_t *new_a = (seq_t *)realloc(a_cache, sz * sizeof(seq_t));
    if (!new_a) abort();

    /* Zero all entries and set a(0) = 1. */
    for (size_t i = 0; i < sz; ++i) new_a[i] = 0;
    new_a[0] = (seq_t)1;

    /* Temporary array for the current F_k(m). */
    seq_t *coeff = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!coeff) abort();
    coeff[0] = (seq_t)1;   /* F_0(0) = 1 */

    idx_t kmax = int_sqrt(n);
    for (idx_t k = 1; k <= kmax; ++k) {
        /* Update coeff to F_k. */
        for (idx_t m = k; m <= n; ++m) {
            coeff[m] -= coeff[m - k];
        }
        /* Add contribution of this k to the sequence. */
        idx_t k2 = k * k;
        for (idx_t idx = k2; idx <= n; ++idx) {
            new_a[idx] += coeff[idx - k2];
        }
    }

    free(coeff);

    a_cache = new_a;
    cache_n  = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
