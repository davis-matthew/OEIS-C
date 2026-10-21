/*
 * A201158  Expansion of e.g.f. exp(x)/(cos(x) - sin(x)).
 * https://oeis.org/A201158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - Σ_{k=0}^{n-1} binom(n,k)·a(k)·b_{n-k} where b_m =
 *            (-1)^{m/2} if m even, (-1)^{(m-1)/2+1} if m odd. Initial value:
 *            a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard header for malloc/realloc. */
#include <stdlib.h>

/* Static cache for memoisation. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* capacity of the allocated array */

/* Grow the cache array to hold at least 'need' entries. */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold the requested term. */
    ensure_cache_size((size_t)n + 1);

    /* Initialise the first known term. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t pos = (seq_t)0;   /* sum of terms with positive sign */
        seq_t neg = (seq_t)0;   /* sum of terms with negative sign */
        seq_t C  = (seq_t)1;    /* binom(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* Determine the sign of b_{m-k}. */
            idx_t nk = m - k;
            int sign;
            if ((nk & 1) == 0) {               /* even index */
                idx_t half = nk >> 1;
                sign = (half & 1) ? -1 : 1;   /* (-1)^half */
            } else {                           /* odd index */
                idx_t half = (nk - 1) >> 1;
                sign = ((half + 1) & 1) ? -1 : 1;  /* (-1)^{half+1} */
            }

            /* Accumulate the term a(k)*C with the correct sign. */
            seq_t term = cache[k] * C;
            if (sign > 0)
                pos += term;
            else
                neg += term;

            /* Update binomial coefficient for the next k. */
            if (k + 1 < m) {
                C *= (seq_t)(m - k);
                C /= (seq_t)(k + 1);
            }
        }

        /* a(m) = 1 + neg - pos   (guaranteed non‑negative). */
        cache[m] = ((seq_t)1 + neg) - pos;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
