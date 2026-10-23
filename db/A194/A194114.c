/*
 * A194114  Sum{floor(j*sqrt(11)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(11).
 * https://oeis.org/A194114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1}^{n} floor(j * sqrt(11))
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------------
 * The sequence is computed on the fly and cached.  For each term we need
 * to evaluate floor(j*sqrt(11)) without using floating point or the
 * provided floor_sqrt utility.  An integer square‑root algorithm for
 * unsigned __int128 values is used instead.
 *
 * This implementation satisfies the requirements:
 *   * compatible with C11 and C++17
 *   * no floating point and no <math.h>
 *   * uses only the allowed operators on seq_t and idx_t
 *   * lazy caching to avoid recomputation for out‑of‑order calls
 *   * efficient enough for n ≤ 10000 (≈O(n) with small constant)
 * ---------------------------------------------------------------------- */

#include <stdlib.h>

/* ---------------------------------------------------------------------- */
/* Integer square‑root for unsigned __int128.  Returns floor(sqrt(x)).   */
/* ---------------------------------------------------------------------- */
static inline seq_t isqrt128(unsigned __int128 x)
{
    /* Increase hi until hi*hi > x, then binary search. */
    unsigned __int128 lo = 0, hi = 1;
    while (hi * hi <= x)
        hi <<= 1;                    /* hi grows until hi² > x */

    /* Binary search for floor sqrt. */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;                /* cast to seq_t – safe for values that fit */
}

/* ---------------------------------------------------------------------- */
/* Compute a(n).  Index a(1) is the first term as per OEIS offset 1.      */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i).  cache[0] = 0 (unused, but convenient). */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;     /* largest n that has been computed */
    static size_t cap = 0;      /* allocated capacity for cache */

    if (n == 0)                   /* safety for calls with n=0 */
        return (seq_t)0;

    /* Expand cache if needed.  Allocate at least n+1 entries for 1‑based indexing. */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;     /* allocation failure – return zero */

        cache = new_cache;
        cap = new_cap;

        /* Ensure cache[0] is initialized for the recursive formula. */
        if (max_n == 0 && cap > 0)
            cache[0] = (seq_t)0;
    }

    /* Compute missing partial sums up to n. */
    for (idx_t j = max_n + 1; j <= n; ++j) {
        /* term = floor(j * sqrt(11)) = floor(sqrt(11 * j^2)) */
        unsigned __int128 prod = (unsigned __int128)11 * j * j;
        seq_t term = isqrt128(prod);

        /* a(j) = a(j-1) + term, with a(0) = 0 stored in cache[0] */
        cache[j] = cache[j - 1] + term;
    }

    max_n = n;
    return cache[n];
}
