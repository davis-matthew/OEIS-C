/*
 * A016212  Expansion of 1/((1-x)*(1-3*x)*(1-7*x)).
 * https://oeis.org/A016212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1 * a(1) = 11 * a(n) = 10*a(n-1) - 21*a(n-2) + 1 for n
 *            ≥ 2 * (This follows from the generating function
 *            1/((1‑x)(1‑3x)(1‑7x)).)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 1080 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *a_cache = NULL;          /* memoized values */
static size_t  a_cache_cap = 0;        /* current capacity of the cache */
static idx_t   a_last_computed = 0;    /* largest index computed so far */

seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n */
    ensure_cache(n, &a_cache, &a_cache_cap);

    /* Initialise first two terms if not yet done */
    if (a_last_computed < 1) {
        a_cache[0] = (seq_t)1;           /* a(0) */
        a_cache[1] = (seq_t)11;          /* a(1) */
        a_last_computed = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = a_last_computed + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)10 * a_cache[i - 1];
        seq_t t2 = (seq_t)21 * a_cache[i - 2];
        a_cache[i] = t1 - t2 + (seq_t)1;
    }
    a_last_computed = n;

    return a_cache[n];
}
