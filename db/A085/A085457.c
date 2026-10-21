/*
 * A085457  Sum_{i=0..n} Sum_{j=0..i} a(j) * a(i-j) = (-11)^n.
 * https://oeis.org/A085457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-6, recurrence: n*a(n) = -2*(5*n-2)*a(n-1) +
 *            11*(n-2)*a(n-2) for n>=2 (equivalently
 *            a(n)=(-2*(5*n-2)*a(n-1)+11*(n-2)*a(n-2))/n)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n) for OEIS A085457.  Works for 8/16/32/64 bit ints and any
 * arbitrary‑precision integer type that implements the usual arithmetic
 * operators. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache of computed values          */
    static size_t cache_cap = 0;    /* allocated capacity                 */
    static idx_t  last_idx  = (idx_t)-1;   /* highest index cached */

    /* Grow the cache if necessary.  We keep it large enough to hold
     * all indices up to the latest request. */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms once. */
    if (last_idx == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)(-6);
        last_idx = (cache_cap > 1) ? 1 : 0;
    }

    /* Return cached value immediately if possible. */
    if (n <= last_idx) return cache[n];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = last_idx + 1; i <= n; ++i) {
        if (i < 2) {               /* i == 0 or 1 – already initialised */
            cache[i] = (i == 0 ? (seq_t)1 : (seq_t)(-6));
            continue;
        }

        /* Use 128‑bit intermediate only when seq_t is 64‑bit or smaller.
         * For larger seq_t we rely on the language/implementation to
         * handle arbitrarily large integers. */
        if (sizeof(seq_t) <= sizeof(long long)) {
            /* Convert all operands to signed 128-bit before multiplying
             * to avoid overflow when intermediate results exceed 64 bits. */
            __int128 term1 = (__int128)(-2) * (int64_t)(5 * (int64_t)i - 2)
                                               * (int64_t)cache[i - 1];
            __int128 term2 = (__int128)11 * (int64_t)(i - 2) * (int64_t)cache[i - 2];
            __int128 num   = term1 + term2;
            cache[i] = (seq_t)(num / (int64_t)i);
        } else {
            /* Let the arbitrary‑precision type perform the arithmetic. */
            seq_t t1 = - (seq_t)2 * (seq_t)(5 * i - 2) * cache[i - 1];
            seq_t t2 =   (seq_t)11 * (seq_t)(i - 2) * cache[i - 2];
            cache[i] = (t1 + t2) / (seq_t)i;
        }
    }

    last_idx = n;
    return cache[n];
}
