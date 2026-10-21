/*
 * A053503  Number of degree-n permutations of order dividing 16.
 * https://oeis.org/A053503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A053503: Number of degree‑n permutations of order dividing 16.
 *  Formula: a(n) = Σ_{d∈{1,2,4,8,16}, d≤n} C(n-1,d-1)·(d-1)!·a(n-d)
 *  with a(0) = 1. */

#include <stdlib.h>

/* memoization cache */
static seq_t *a_cache   = NULL;  /* array of computed a(n) values */
static idx_t  cache_cap = 0;    /* allocated capacity of a_cache  */
static idx_t  max_calc  = 0;    /* highest n for which a(n) is known */

static const idx_t DIVISORS[5] = {1,2,4,8,16};

seq_t a(idx_t n)
{
    /* Allocate cache if needed. */
    if (cache_cap == 0) {
        cache_cap = 1;
        a_cache   = (seq_t*)malloc(sizeof(seq_t));
        if (!a_cache) return (seq_t)0;          /* allocation failed */
        a_cache[0] = (seq_t)1;                  /* a(0) = 1          */
        max_calc   = 0;
    }
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        a_cache   = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t m = max_calc + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t d = 0; d < 5; ++d) {
            idx_t j = DIVISORS[d];
            if (j > m) continue;
            /* Compute product Π_{k=m-j+1}^{m-1} k   (j-1 factors) */
            seq_t prod = 1;
            for (idx_t k = m - j + 1; k <= m - 1; ++k) {
                prod *= (seq_t)k;
            }
            sum += a_cache[m - j] * prod;
        }
        a_cache[m] = sum;
    }
    max_calc = n;
    return a_cache[n];
}
