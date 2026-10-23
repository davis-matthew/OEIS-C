/*
 * A023429  Generalized Catalan Numbers x^4*A(x)^2 -(1-x+x^4+x^5+x^6)*A(x) + 1
 *          =0.
 * https://oeis.org/A023429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A023429 satisfies the functional equation * x^4*A(x)^2
 *            -(1-x+x^4+x^5+x^6)*A(x) + 1 = 0. * Recurrence: a(0) = 1. * For n
 *            ≥ 1: * a(n) = a(n-1) + c(n-4) - a(n-4) - a(n-5) - a(n-6) * where
 *            c(m) = Σ_{k=0}^{m} a(k)*a(m-k) (the convolution of A with
 *            itself). * This implementation caches intermediate val...
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 47, u64 n <= 86, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;        /* stored a[n] values           */
static size_t  a_used   = 0;         /* highest index computed      */
static size_t  a_cap    = 0;         /* current cache capacity      */

/* Helper to grow the cache to at least size 'n+1' */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= a_cap)
        return;
    size_t new_cap = ((size_t)n + 1) * 2;
    seq_t *new_a = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_a) { a_cache = NULL; a_cap = 0; return; }
    /* initialize newly allocated part */
    for (size_t i = a_cap; i < new_cap; ++i)
        new_a[i] = 0;
    a_cache = new_a;
    a_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if ((size_t)n < a_used)      /* already computed */
        return a_cache[n];

    ensure_cache(n);

    /* compute missing values up to n */
    for (size_t i = a_used; i <= (size_t)n; ++i)
    {
        if (i == 0)
        {
            a_cache[0] = 1;
            continue;
        }

        /* compute convolution sum c(i-4) if needed */
        seq_t sumsq = 0;
        if (i >= 4)
        {
            idx_t m = (idx_t)(i - 4);
            for (idx_t k = 0; k <= m; ++k)
                sumsq += a_cache[(size_t)k] * a_cache[(size_t)(m - k)];
        }

        /* part1 = a(i-1) + sumsq (if i>=4) */
        seq_t part1 = a_cache[(size_t)(i - 1)];
        if (i >= 4)
            part1 += sumsq;

        /* part2 = a(i-4) + a(i-5) + a(i-6) (only if indices >=0) */
        seq_t part2 = 0;
        if (i >= 4) part2 += a_cache[(size_t)(i - 4)];
        if (i >= 5) part2 += a_cache[(size_t)(i - 5)];
        if (i >= 6) part2 += a_cache[(size_t)(i - 6)];

        a_cache[i] = part1 - part2;
    }

    a_used = (size_t)n + 1;
    return a_cache[n];
}
