/*
 * A182304  G.f. satisfies: A(x) = x + d/dx A(x)^3/3.
 * https://oeis.org/A182304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n ≥ 2: a(n) = Σ_{m=1}^{n-1} m · a(m) · c_{n-m+1},
 *            where c_k = Σ_{i=1}^{k-1} a(i) · a(k-i). These formulas come
 *            from the differential equation A(x) = x + A'(x)·A(x)²
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cached values of the sequence and of the convolution array c_k */
static seq_t *a_cache   = NULL;   /* a[1..] , a_cache[0] unused */
static seq_t *c_cache   = NULL;   /* c[2..] , c_cache[0],c_cache[1] unused */
static size_t a_cap = 0;           /* allocated size for a_cache   */
static size_t c_cap = 0;           /* allocated size for c_cache   */
static idx_t  max_n = 0;           /* largest n that has been computed */

static void ensure_a_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* indices 0..n, we use 1..n */
    if (a_cap < need) {
        seq_t *tmp;
        if (a_cache == NULL)
            tmp = seq_alloc(need);
        else
            tmp = seq_realloc(a_cache, a_cap, need);
        if (tmp == NULL) return;        /* gracefully handle OOM */

        /* Zero newly allocated elements */
        for (size_t i = a_cap; i < need; ++i)
            tmp[i] = 0;
        a_cache = tmp;
        a_cap   = need;
    }
}

static void ensure_c_cache(idx_t n)
{
    size_t need = (size_t)(2 * n) + 1;   /* we need indices up to 2n */
    if (c_cap < need) {
        seq_t *tmp;
        if (c_cache == NULL)
            tmp = seq_alloc(need);
        else
            tmp = seq_realloc(c_cache, c_cap, need);
        if (tmp == NULL) return;        /* gracefully handle OOM */

        /* Zero newly allocated elements */
        for (size_t i = c_cap; i < need; ++i)
            tmp[i] = 0;
        c_cache = tmp;
        c_cap   = need;
    }
}

seq_t a(idx_t n)
{
    /* guard against out‑of‑range requests (unlikely but safe) */
    if (n == 0) return 0;

    /* Ensure the dynamic arrays are large enough */
    ensure_a_cache(n);
    ensure_c_cache(n);

    /* Initialise base case if nothing has been computed yet */
    if (max_n == 0) {
        a_cache[1] = 1;                     /* a(1) */
        /* c(2) = a1*a1 */
        if (c_cap < 3) { /* just in case, though ensured above */
            seq_t *tmp = seq_realloc(c_cache, c_cap, 3);
            if (tmp) {
                for (size_t i = c_cap; i < 3; ++i) tmp[i] = 0;
                c_cache = tmp;
                c_cap   = 3;
            }
        }
        c_cache[2] = a_cache[1] * a_cache[1];
        max_n = 1;
    }

    /* Return cached value if already available */
    if (n <= max_n)
        return a_cache[(size_t)n];

    /* Compute terms from max_n+1 up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* Calculate a(m) */
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            seq_t term = (seq_t)k * a_cache[(size_t)k];
            term *= c_cache[(size_t)(m - k + 1)];
            sum += term;
        }
        a_cache[(size_t)m] = sum;

        /* Update the convolution array c for future indices */
        for (idx_t j = 1; j < m; ++j) {
            size_t idx = (size_t)(m + j);   /* c_{m+j} */
            seq_t add = (seq_t)2 * a_cache[(size_t)m] * a_cache[(size_t)j];
            c_cache[idx] += add;
        }
        /* c_{2m} receives the a(m)*a(m) term */
        size_t idx2m = (size_t)(2 * m);
        c_cache[idx2m] += a_cache[(size_t)m] * a_cache[(size_t)m];
    }

    max_n = n;
    return a_cache[(size_t)n];
}
