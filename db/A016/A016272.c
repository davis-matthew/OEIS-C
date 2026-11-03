/*
 * A016272  Inverse of 2263rd cyclotomic polynomial.
 * https://oeis.org/A016272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of 1/Φ_2263(x). The coefficients satisfy the linear recurrence
 *            a_0 = 1 a_n = a_{n-1} + a_{n-2263} - a_{n-2264} + rhs_n, where
 *            rhs_0 = +1 rhs_31 = -1 rhs_73 = -1 rhs_104 = +1 rhs_k = 0
 *            otherwise. This recurrence identifies the sequence...
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Validated: standard, pass, 97 terms through n = 2896 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache          = NULL;  /* array holding terms a[0..size-1]    */
static size_t cache_cap      = 0;      /* allocated capacity                 */
static size_t cache_used     = 0;      /* number of terms already computed   */

/* Ensure that cache has capacity for at least (n+1) terms. */
static void
ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (cache_cap < required) {
        size_t new_cap = (cache_cap == 0 ? 64 : cache_cap);
        while (new_cap < required)
            new_cap <<= 1;           /* grow geometrically               */
        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Compute a(n).  Uses memoisation so that each index is only computed once. */
seq_t a(idx_t n)
{
    /* Expand cache if necessary. */
    if (cache_used <= n) {
        ensure_capacity(n);
        /* Compute terms up to n. */
        for (idx_t i = cache_used; i <= n; ++i) {
            seq_t val = 0;
            if (i > 0)
                val += cache[(size_t)(i - 1)];
            if (i >= 2263)
                val += cache[(size_t)(i - 2263)];
            if (i >= 2264)
                val -= cache[(size_t)(i - 2264)];
            /* RHS contributions */
            if (i == 0)       val += 1;
            if (i == 31)      val -= 1;
            if (i == 73)      val -= 1;
            if (i == 104)     val += 1;
            cache[(size_t)i] = val;
        }
        cache_used = n + 1;
    }
    return cache[(size_t)n];
}
