/*
 * A115412  G.f.: (x - 1)/(x^5 - x^3 - x^2 - x - 1).
 * https://oeis.org/A115412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            -a(n-1)-a(n-2)-a(n-3)+a(n-5) + rhs(n) with rhs(0)= 1, rhs(1)=
 *            -1, rhs(n)=0 for n≥2. Initial values: a(0)=1, a(1)=-2, a(2)=1,
 *            a(3)=0, a(4)=1
 * Range:     i8 n <= 32, i16 n <= 66, i32 n <= 134, i64 n <= 270,
 *            i128 n <= 541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>

/* Global cache for memoised values */
static seq_t *a_cache = NULL;      /* array of values */
static idx_t  a_cache_len = 0;     /* number of computed values */

/* Helper to ensure cache holds values up to n (inclusive) */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_len) return;              /* already computed */

    /* Grow the cache to hold values up to n */
    seq_t *new_cache = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) {                          /* allocation failure -- abort */
        exit(1);
    }
    a_cache = new_cache;

    /* Compute missing terms */
    for (idx_t i = a_cache_len; i <= n; ++i) {
        seq_t rhs = 0;
        if (i == 0) rhs = (seq_t)1;
        else if (i == 1) rhs = -(seq_t)1;

        seq_t a_m1 = (i >= 1 ? a_cache[i - 1] : (seq_t)0);
        seq_t a_m2 = (i >= 2 ? a_cache[i - 2] : (seq_t)0);
        seq_t a_m3 = (i >= 3 ? a_cache[i - 3] : (seq_t)0);
        seq_t a_m5 = (i >= 5 ? a_cache[i - 5] : (seq_t)0);

        seq_t term = rhs - a_m1 - a_m2 - a_m3 + a_m5;
        a_cache[i] = term;
    }

    a_cache_len = n + 1;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    /* Copyright: compute up to n using cache */
    ensure_a_cache(n);
    return a_cache[n];
}
