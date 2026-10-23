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
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

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
