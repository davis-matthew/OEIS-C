/*
 * A184018  Expansion of c(x/(1-x-x^2)) / (1-x-x^2), c(x) the g.f. of A000108.
 * https://oeis.org/A184018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1: a(n) = f(n) + Σ_{i=0}^{n-1} a(i)*a(n-1-i) where
 *            f(n) are the coefficients of 1/(1‑x‑x²) ⇔ f(0)=1, f(1)=1,
 *            f(n)=f(n-1)+f(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* a[0..] */
static seq_t *fib   = NULL;      /* f[0..] */
static idx_t  cap   = 0;         /* allocated capacity */
static idx_t  next  = 0;         /* next index to compute */

/* Grow the arrays so that indices < need are valid. */
static void ensure_alloc(idx_t need)
{
    if (need <= cap)
        return;
    idx_t new_cap = cap ? cap : 4;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    fib   = (seq_t *)realloc(fib,   new_cap * sizeof(seq_t));
    /* initialise new entries to zero */
    for (idx_t i = cap; i < new_cap; ++i) {
        cache[i] = 0;
        fib[i]   = 0;
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (cache == NULL) {
        ensure_alloc(4);
        /* first term */
        cache[0] = 1;
        fib[0]   = 1;
        next     = 1;      /* next index still to be computed */
    }

    /* already computed? */
    if (n < next)
        return cache[n];

    ensure_alloc(n + 1);

    for (idx_t i = next; i <= n; ++i) {
        /* Fibonacci part */
        if (i == 1)
            fib[i] = 1;          /* fib[1] = 1 */
        else if (i >= 2)
            fib[i] = fib[i-1] + fib[i-2];

        /* convolution sum */
        seq_t sum = 0;
        if (i > 0) {
            for (idx_t k = 0; k < i; ++k)
                sum += cache[k] * cache[i-1-k];
        }

        /* recurrence */
        cache[i] = fib[i] + sum;
    }

    next = n + 1;
    return cache[n];
}
