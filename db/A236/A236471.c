/*
 * A236471  Riordan array ((1-x)/(1-2*x), x(1-x)/(1-2*x)^2).
 * https://oeis.org/A236471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened row‑major entries of the Riordan array * with
 *            generating functions * g(x) = (1-x)/(1-2x), h(x) =
 *            x(1-x)/(1-2x)^2. * The triangle T(n,k) satisfies * T(n,k) =
 *            Σ_{m=0..n} C(m+k, 2k) * C(n-1, n-m) * and a(n) lists the entries
 *            of T in row‑major order. * * This implementation builds th...
 * Range:     u8 n <= 21, u16 n <= 67, u32 n <= 195, u64 n <= 712,
 *            u128 n <= 2646, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial.h"    /* seq_t binomial(idx_t a, idx_t b); */

/* ---------- cache ------------------------------------------- */
static seq_t *cache = NULL;          /* flattened table of T(n,k) */
static size_t cache_cap = 0;         /* allocated capacity in seq_t's */
static size_t cache_len = 0;         /* number of filled entries      */
static idx_t max_row_computed = (idx_t)-1;   /* last fully computed row   */

/* ---------- helper: floor sqrt of 64‑bit unsigned ------------- */
static inline idx_t floor_sqrt_u64(idx_t n)
{
#if defined(_floor_isqrt_u64)
    return _floor_isqrt_u64(n);
#else
    /* n ≤ 8 * 10⁴ for the required range; double is exact here. */
    return (idx_t)floor(sqrt((long double)n));
#endif
}

/* ---------- ensure capacity ---------------------------------- */
static void ensure_cache_capacity(size_t n)
{
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();          /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }
}

/* ---------- compute a single row of the triangle -------------- */
static void compute_row(idx_t r)
{
    idx_t start = r * (r + 1) / 2;

    /* ensure we can write the whole row */
    idx_t end_idx = start + r;       /* last index of the row   */
    ensure_cache_capacity((size_t)end_idx);

    /* special case r == 0 */
    if (r == 0) {
        cache[start] = (seq_t)1;
        return;
    }

    for (idx_t k = 0; k <= r; ++k) {
        seq_t val = (seq_t)0;
        for (idx_t m = 0; m <= r; ++m) {
            seq_t c1 = binomial(m + k, 2 * k);          /* C(m+k,2k)  */
            seq_t c2 = (m == 0) ? (seq_t)0
                                 : binomial(r - 1, r - m); /* C(r-1,r-m) */
            val += c1 * c2;
        }
        cache[start + k] = val;
    }
}

/* ---------- public function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* ensure at least the first element is stored */
        ensure_cache_capacity(0);
        if (cache_len == 0) {
            compute_row(0);
            max_row_computed = 0;
            cache_len = 1;
        }
        return cache[0];
    }

    /* ensure cache large enough to contain index n */
    ensure_cache_capacity((size_t)n);

    if (n >= cache_len) {
        /* find the row that contains index n */
        idx_t square = floor_sqrt_u64(8 * n + 1);
        idx_t target_row = (square - 1) / 2;

        /* compute all rows up to target_row */
        for (idx_t r = max_row_computed + 1; r <= target_row; ++r) {
            compute_row(r);
        }
        max_row_computed = target_row;
        cache_len = (size_t)(target_row + 1) * (target_row + 2) / 2;
    }

    return cache[n];
}
