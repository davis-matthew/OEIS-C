/*
 * A202947  G.f.: [ Sum_{n>=0} (n+1) * 2^(n^2) * x^n ]^(1/2).
 * https://oeis.org/A202947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*2^(n^2-1) - Σ_{k=1}^{n-1} a(n-k)*a(k)/2 for n>0,
 *            a(0)=1 Equivalent convolution recurrence: Σ_{k=0}^{n}
 *            a(k)*a(n-k) = (n+1)*2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes values on demand, memoising the sequence in a static
   array so that calls can arrive in any order.  It uses the utility
   functions `pow2` (to compute 2^exp) and `ensure_cache` (to resize the
   cache).  The implementation is purely integer based and works for built‑in
   unsigned types as well as for arbitrary‑precision integer classes. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of elements actually stored */
    static size_t cache_cap = 0;      /* current allocated capacity */

    /* make sure the cache can hold values up to index n */
    ensure_cache(n, &cache, &cache_cap);

    /* compute any missing entries in increasing order */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                /* base case */
            cache[0] = 1;
            continue;
        }

        /* S = Σ_{k=1}^{i-1} a(k) * a(i-k) */
        seq_t S = 0;
        for (idx_t k = 1; k <= i - 1; ++k) {
            S += cache[k] * cache[i - k];
        }

        /* target = (i+1) * 2^(i^2) - S */
        seq_t target = (seq_t)(i + 1) * pow2((idx_t)(i * i));

        target -= S;                   /* subtraction */

        /* a(i) = target / 2 */
        cache[i] = target >> 1;        /* integer division by 2 */
    }

    /* update the cached length to reflect that indices up to n are now valid */
    cache_len = (size_t)n + 1;

    return cache[n];
}
