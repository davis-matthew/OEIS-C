/*
 * A122393  Dimension of 4-variable non-commutative harmonics (Hausdorff
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 4 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( w ) = sum
 *          over all subwords of w deleting xi once).
 * https://oeis.org/A122393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122393: Dimension of 4‑variable non‑commutative harmonics.
 *
 * Generating function:  G(q) = (1-q)(1-q²)(1-q³)(1-q⁴)/(1-4q).
 *
 * Recurrence (valid for all n ≥ 1):
 *   a(0) = 1
 *   a(n) = 4*a(n-1) + c(n)
 *   where c(n) = { -1,-1,0,0,2,0,0,-1,-1,1 } for n = 1…10
 *   and c(n) = 0 for n > 10.
 *
 * This gives a(n) = 4*a(n-1) for all n > 10, which matches the
 * explicit formula a(n) = 722925 * 4^(n-10) for n > 9.
 *
 * The implementation caches computed terms so that calls can be in
 * arbitrary order.  All arithmetic is performed in the type seq_t,
 * which is supplied by the build system and may be a built‑in unsigned
 * integer of various sizes or an arbitrary–precision integer class.
 */

#include <stdlib.h>
#include <stddef.h>

/* Coefficient array for c(n), 0 is unused. */
static const int coeff[11] = { 0, -1, -1, 0, 0, 2, 0, 0, -1, -1, 1 };

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Ensure that the cache can hold indices up to n. */
static void ensure_capacity(idx_t n, size_t *cap, seq_t **arr)
{
    if (*cap <= (size_t)n) {
        size_t new_cap = (*cap == 0) ? 1 : *cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;   /* multiply by 2 */
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_max = 0;
    }

    /* If the value is already cached, return it. */
    if (n <= cache_max)
        return cache[n];

    /* Ensure the cache can store up to n. */
    ensure_capacity(n, &cache_cap, &cache);

    /* Compute and cache terms from (cache_max+1) through n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)4;
        if (i <= 10)
            val += coeff[i];   /* apply polynomial coefficient */
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
