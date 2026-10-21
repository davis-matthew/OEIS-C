/*
 * A058006  Alternating factorials: 0! - 1! + 2! - ... + (-1)^n n!
 * https://oeis.org/A058006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + (-1)^n * n!, a(0) = 1 This implementation
 *            precomputes factorials and values up to the largest n requested
 *            among all calls (memoisation). All arithmetic uses the native
 *            seq_t type, which may be a built‑in integer or an
 *            arbitrary‑precision class in a C++ build. The code is fully
 *            com...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the alternating factorial sum a(n) = Σ_{k=0..n} (-1)^k * k! */
seq_t a(idx_t n)
{
    /* Static cache for computed a(n) and factorials. */
    static idx_t  up_to    = 0;      /* highest index computed so far       */
    static idx_t  cache_sz = 0;      /* allocated cache size                */
    static seq_t *a_cache  = NULL;   /* values of a(n)                      */
    static seq_t *fact     = NULL;   /* factorials k! (k <= up_to)          */

    /* Initialisation on first call. */
    if (cache_sz == 0) {
        cache_sz = 1; /* at least space for a(0) */
        a_cache  = (seq_t*)malloc(sizeof(seq_t) * cache_sz);
        fact     = (seq_t*)malloc(sizeof(seq_t) * cache_sz);
        a_cache[0] = (seq_t)1;   /* a(0) = 0! = 1  */
        fact[0]    = (seq_t)1;   /* 0! = 1          */
        up_to      = 0;
    }

    /* Ensure the cache is large enough for n. */
    if (n >= cache_sz) {
        idx_t new_sz = cache_sz * 2;
        if (n + 1 > new_sz) new_sz = n + 1;
        a_cache = (seq_t*)realloc(a_cache, sizeof(seq_t) * new_sz);
        fact    = (seq_t*)realloc(fact,    sizeof(seq_t) * new_sz);
        cache_sz = new_sz;
    }

    /* Compute all missing indices up to n. */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        /* factorial: fact[i] = fact[i-1] * i */
        fact[i] = fact[i - 1] * (seq_t)i;

        /* term = (-1)^i * fact[i] */
        seq_t term = fact[i];
        if (i & 1U)   /* odd i -> subtract */
            term = -term;

        /* recurrence: a[i] = a[i-1] + term */
        a_cache[i] = a_cache[i - 1] + term;
    }

    up_to = n;
    return a_cache[n];
}
