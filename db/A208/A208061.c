/*
 * A208061  G.f. 1/sum(k>=0, (-1)^k * x^(k*(k+1)/2)).
 * https://oeis.org/A208061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 45, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208061: coefficients of 1 / sum_{k>=0} (-1)^k x^{k(k+1)/2}
   Recurrence:
     a(0) = 1;
     a(n) = - Σ_{m>=1, t=m(m+1)/2 ≤ n} (-1)^m · a(n – t)   for n > 0
   where t is a triangular number.  The array of computed values is cached
   so that repeated calls in any order are fast. */

#include <stdlib.h>   /* size_t, NULL */
#include <stddef.h>   /* ptrdiff_t */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.  The array grows as needed.
       The implementation uses C++ 'new' and 'delete[]' when compiling as C++,
       and malloc/free when compiling as C.  Because seq_t may be a plain
       integer or an arbitrary‑precision class, we cannot use realloc for
       the latter.  The conditional compilation below chooses the appropriate
       allocation strategy. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* next free index in cache (== number of cached terms) */
    static idx_t cache_cap = 0;   /* capacity of the allocated array */

    /* Allocate or expand the cache to accommodate indices up to n.  We
       double the capacity each time to keep arithmetic O(n). */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : (idx_t)64;
        while (new_cap < n + 1) new_cap <<= 1;

        seq_t *new_cache = NULL;
#ifndef __cplusplus
        /* C: use malloc.  The elements are trivially copyable. */
        new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
#else
        /* C++: use new, which calls default constructors. */
        new_cache = new seq_t[(size_t)new_cap];
#endif
        /* Copy any existing values. */
        for (idx_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old storage. */
#ifndef __cplusplus
        free(cache);
#else
        delete[] cache;
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize a(0) if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute all terms up to n that are not yet cached. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t m = 1;; ++m) {
            idx_t t = m * (m + 1) / 2;            /* triangular number */
            if (t > i) break;
            seq_t term = cache[i - t];
            if (m & (idx_t)1) term = -term;      /* (-1)^m factor */
            sum += term;
        }
        cache[i] = -sum;                          /* a(i) = -Σ ... */
    }
    cache_len = n + 1;

    return cache[n];
}
