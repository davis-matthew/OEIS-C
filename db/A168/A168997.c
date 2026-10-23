/*
 * A168997  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168997
   a(0)=1,
   a(1)=32, a(2)=992, …
   For 0<n<23:  a(n) = p_n + 30·∑_{i=0}^{n-1} a(i),
       where p_0 = 1 and p_n = 2 for 1≤n≤22.
   For n≥23   :  a(n) = 30·∑_{i=1}^{22} a(n-i) – 465·a(n-23). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Memoisation buffer for the sequence terms */
static seq_t *cache   = NULL; /* array of terms */
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t   cache_len = 0;  /* highest computed index + 1 */

/* Ensure that a(i) is available for all i∈[0,n] */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) /* already computed */
        return;

    /* Grow buffer if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = needed * 2;
        if (new_cap < needed)        /* integer overflow guard */
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term;
        if (i < 23) { /* use the initial‑segment formula */
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[(size_t)j];
            term = (i == 0 ? (seq_t)1 : (seq_t)2) + (seq_t)30 * sum;
        } else { /* recurrence for i ≥ 23 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 22; ++k)
                sum += cache[(size_t)(i - k)];
            term = (seq_t)30 * sum - (seq_t)465 * cache[(size_t)(i - 23)];
        }
        cache[(size_t)i] = term;
    }
    cache_len = n + 1;
}

/* External API required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
