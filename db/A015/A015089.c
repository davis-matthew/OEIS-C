/*
 * A015089  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=6.
 * https://oeis.org/A015089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = Σ_{i=0..n} 6^i * a(i) * a(n-i), with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static idx_t  cache_last = 0;    /* largest index already computed   */
    static size_t cache_cap  = 0;    /* current allocated capacity       */

    /* static cache for powers of q = 6 */
    static seq_t *qpow      = NULL;
    static idx_t  qpow_last = 0;     /* largest index already computed   */
    static size_t qpow_cap  = 0;     /* current allocated capacity       */

    /* Constants – use the type seq_t for compatibility with big builds */
    const seq_t q = 6;        /* the fixed q in the recurrence */

    /* ------------------------------------------------------------------ */
    /* 1. Allocate/resize the term cache if necessary.                   */
    /* ------------------------------------------------------------------ */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* allocate (or enlarge) using helper that works for built‑in and
           arbitrary precision types */
        cache = seq_realloc(cache, cache_cap, new_cap);
        qpow = seq_realloc(qpow, qpow_cap, new_cap);

        /* Initialize the first elements on first allocation */
        if (cache_cap == 0) {
            cache[0] = 1;        /* a(0) = 1                      */
            qpow[0] = 1;        /* 6^0 = 1                        */
            cache_last = 0;
            qpow_last = 0;
        }
        cache_cap = qpow_cap = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* 2. Pre‑compute powers of 6 up to index n, if not already computed.*/
    /* ------------------------------------------------------------------ */
    if (n > qpow_last) {
        for (idx_t k = qpow_last + 1; k <= n; ++k) {
            qpow[k] = qpow[k - 1] * q;
        }
        qpow_last = n;
    }

    /* ------------------------------------------------------------------ */
    /* 3. Compute new terms using the recurrence:                         */
    /*      a(m) = Σ_{i=0}^{m-1} 6^i * a(i) * a(m-1-i)                     */
    /*        (note: you can equivalently use a(m+1) = Σ_{i=0}^m ...).      */
    /* ------------------------------------------------------------------ */
    for (idx_t m = cache_last + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i) {
            seq_t term = qpow[i] * cache[i];
            term *= cache[m - 1 - i];
            sum += term;
        }
        cache[m] = sum;
    }
    cache_last = n;

    return cache[n];
}
