/*
 * A042358  Numerators of continued fraction convergents to sqrt(706).
 * https://oeis.org/A042358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence : A042358
 * Description : Numerators of the continued‑fraction convergents to √706.
 * Formula : a(0..15) are computed from the continued fraction of √706.
 * For n ≥ 16 : a(n) = 69190 · a(n‑8) – a(n‑16)
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Static cache for previously computed terms */
static seq_t *cache     = NULL; /* array a[0] … a[last] */
static size_t cache_cap = 0;    /* allocated capacity (in terms) */
static size_t cache_len = 0;    /* number of terms actually stored */

/* Initialise the cache on the first call and resize if necessary.   */
static void init_cache(idx_t n)
{
    /* First call: allocate space and compute the first 16 terms from the CF. */
    if (cache == NULL) {
        /* Ensure at least 16 entries, or more if n is larger. */
        cache_cap = (size_t)n + 1;
        if (cache_cap < 16) cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;                /* allocation failed – caller will fail later */

        /* Compute the first 16 numerators from the continued‑fraction of √706. */
        const seq_t sqrt706 = floor_sqrt((seq_t)706);
        seq_t m   = 0;
        seq_t d   = 1;
        seq_t a0  = sqrt706;           /* a0 = ⌊√706⌋ */
        seq_t a_prev = a0;             /* previous partial quotient */
        seq_t p_prev2 = 0;             /* p_{k‑2}  */
        seq_t p_prev1 = 1;             /* p_{k‑1}  */

        for (idx_t k = 0; k < 16; ++k) {
            seq_t a_k;

            if (k == 0) {
                a_k = a0;
            } else {
                /* Standard continued‑fraction recursion */
                m = d * a_prev - m;
                d = (((seq_t)706) - m * m) / d;
                a_k = (a0 + m) / d;   /* ⌊(a0 + m)/d⌋ */
            }

            /* Convergent numerator recurrence: p_k = a_k·p_{k‑1} + p_{k‑2} */
            const seq_t p_k = a_k * p_prev1 + p_prev2;
            cache[k] = p_k;

            /* Shift for next iteration */
            p_prev2 = p_prev1;
            p_prev1 = p_k;
            a_prev  = a_k;
        }

        cache_len = 16;  /* first 16 terms are now stored */
    }
    /* Subsequent calls: resize if n is outside the current capacity. */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;  /* allocation failed – caller will handle it */
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* Return the nth term of A042358.  */
seq_t a(idx_t n)
{
    init_cache(n);
    if (!cache) return (seq_t)0;        /* allocation failure */

    /* If the term is already computed, return it. */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Compute new terms up to n using the linear recurrence:  a(i) = 69190·a(i‑8) – a(i‑16) */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 69190 * cache[(size_t)(i - 8)] - cache[(size_t)(i - 16)];
        cache[(size_t)i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
