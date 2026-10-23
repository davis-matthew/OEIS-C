/*
 * A128079  a(n) = Sum_{k=0..n} A000984(k)*A001263(n+1,k+1), where A000984 is
 *          the central binomial coefficients and A001263 is the Narayana
 *          triangle.
 * https://oeis.org/A128079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A128079: a(n) = Sum_{k=0..n} C(2k,k) * C(n,k) * C(n+1,k) / (k+1)
 * Recurrence: (n+1)(n+2)a(n) = (7n^2+11n+6)a(n-1)
 *                + 3(7n^2-19n+6)a(n-2)
 *                - 27(n-2)(n-1)a(n-3)
 * Initial values: a(0) = 1, a(1) = 3, a(2) = 13.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__SIZEOF_INT128__)
#  define HAVE_INT128 1
#else
#  define HAVE_INT128 0
#endif

/* Cache for computed values */
static seq_t *cache      = NULL;   /* array of a(i) */
static idx_t  cache_cap  = 0;      /* allocated size of cache */
static idx_t  cache_size = 0;      /* number of values written */

/* Ensure cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) return;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)13;
        cache_size = 3;
    }

    /* Nothing to do if we already have values up to n */
    if (cache_size > n) return;

    /* Grow cache array if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return;          /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i < 3) {           /* first three terms are known */
            if (i == 0) val = (seq_t)1;
            else if (i == 1) val = (seq_t)3;
            else            val = (seq_t)13;
        } else {
            /* Prefer 128‑bit arithmetic when seq_t is small enough */
            if (HAVE_INT128 && sizeof(seq_t) <= 8) {
                unsigned __int128 ii   = (unsigned __int128)i * (unsigned __int128)i;
                unsigned __int128 termA = 7 * ii + 11 * (unsigned __int128)i + 6;
                unsigned __int128 termB = 7 * ii - 19 * (unsigned __int128)i + 6;

                unsigned __int128 num = termA * (unsigned __int128)cache[i-1];
                num += 3 * termB * (unsigned __int128)cache[i-2];
                num -= 27 * (unsigned __int128)(i-2) *
                       (unsigned __int128)(i-1) *
                       (unsigned __int128)cache[i-3];

                unsigned __int128 denom = (unsigned __int128)(i+1) *
                                          (unsigned __int128)(i+2);
                val = (seq_t)(num / denom);   /* exact division */
            } else {
                /* Use seq_t arithmetic directly (works for arbitrary‑precision) */
                seq_t ii   = (seq_t)i * (seq_t)i;
                seq_t termA = 7 * ii + 11 * (seq_t)i + 6;
                seq_t termB = 7 * ii - 19 * (seq_t)i + 6;

                seq_t num = termA * cache[i-1];
                num += (seq_t)3 * termB * cache[i-2];
                num -= (seq_t)27 * (seq_t)(i-2) *
                       (seq_t)(i-1) *
                       cache[i-3];

                seq_t denom = (seq_t)(i+1) * (seq_t)(i+2);
                val = num / denom;   /* exact division */
            }
        }
        cache[i] = val;
    }
    cache_size = n + 1;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_size) return cache[n];
    /* Should never happen unless allocation failed */
    return (seq_t)0;
}
