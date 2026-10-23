/*
 * A152730  a(n) + a(n+1) + a(n+2) = n^5, with a(1) = a(2) = 0.
 * https://oeis.org/A152730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) + a(n+1) + a(n+2) = n^5, with a(1) = a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 106, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute n^5 using 128‑bit intermediate to avoid overflow on 64‑bit builds. */
static inline seq_t pow5(idx_t n)
{
#if defined(__SIZEOF_INT128__) && defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    __uint128_t r = n;
    r *= n; r *= n; r *= n; r *= n;
    return (seq_t)r;
#else
    seq_t r = (seq_t)n;
    r *= (seq_t)n;
    r *= (seq_t)n;
    r *= (seq_t)n;
    r *= (seq_t)n;
    return r;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but return 0 */

    /* static memoization table: cache[0] = a(1), cache[1] = a(2) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of stored elements (a(1)...a(cache_size)) */

    /* initialise cache if necessary */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache_size = 2;
    }

    if (n <= 2) return (seq_t)0;

    /* keep old size to know where to start computing */
    idx_t old_size = cache_size;
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_size = n;
    }

    /* Compute missing terms up to a(n) */
    for (idx_t i = old_size + 1; i <= n; ++i) {
        /* a(i) = (i-2)^5 - a(i-2) - a(i-1)   for i >= 3 */
        seq_t val = pow5(i - 2);
        val -= cache[(i - 3)]; /* a(i-2) */
        val -= cache[(i - 2)]; /* a(i-1) */
        cache[i - 1] = val;    /* store a(i) */
    }

    return cache[n - 1]; /* a(n) */
}
