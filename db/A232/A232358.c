/*
 * A232358  Expansion of phi(q^2)^2 / (phi(q) * phi(q^4)) in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A232358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 70, i64 n <= 248, i128 n <= 912,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 2238 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232358 – Expansion of φ(q²)²/(φ(q) φ(q⁴))
   G.f.: Product_{k>0} (1 + x^(4*k-2))^5 / ((1 + x^(2*k-1)) * (1 + x^(8*k-4)))^2
   The implementation builds a memoized linear‑time series using array
   arithmetic. */
#include <stdlib.h>
#include "../utils/mem.h"

static void divide_by_plus_one(seq_t *a, idx_t N, idx_t r)
{
    for (idx_t i = r; i <= N; ++i) {
        seq_t old = a[i];
        a[i] = old - a[i - r];
    }
}

static void multiply_by_plus_one(seq_t *a, idx_t N, idx_t r)
{
    for (idx_t i = N; i >= r; --i) {
        a[i] += a[i - r];
        if (i == r) break;  /* avoid unsigned wrap‑around */
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* Build the series up to n if necessary */
    if (cache == NULL || n > cached_n) {
        /* Allocate space for coefficients 0..n */
        size_t cap = (size_t)(n + 1);
        seq_t *new_cache = mem_new(seq_t, cap);
        if (!new_cache) return 0;  /* allocation failed – return 0 */

        /* Initialise all to 0, except a[0] = 1 */
        for (idx_t i = 0; i <= n; ++i) new_cache[i] = 0;
        new_cache[0] = 1;

        /* 1. Numerator: (1 + x^(4*k - 2))^5 for r ≡ 2 (mod 4) */
        for (idx_t r = 2; r <= n; r += 4) {
            for (int k = 0; k < 5; ++k)
                multiply_by_plus_one(new_cache, n, r);
        }

        /* 2. First part of denominator: (1 + x^(2*k - 1))^2 for odd r */
        for (idx_t r = 1; r <= n; r += 2) {
            divide_by_plus_one(new_cache, n, r);
            divide_by_plus_one(new_cache, n, r);
        }

        /* 3. Second part of denominator: (1 + x^(8*k - 4))^2 for r ≡ 4 (mod 8) */
        for (idx_t r = 4; r <= n; r += 8) {
            divide_by_plus_one(new_cache, n, r);
            divide_by_plus_one(new_cache, n, r);
        }

        /* Replace old cache (if any) */
        mem_free(cache);
        cache = new_cache;
        cached_n = n;
    }

    return cache[n];
}
