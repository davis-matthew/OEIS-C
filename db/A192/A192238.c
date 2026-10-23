/*
 * A192238  Constant term in the reduction of the polynomial
 *          x(x+1)(x+2)...(x+n-1) by x^2 -> x+1.
 * https://oeis.org/A192238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 0, a(3) = 1 For n ≥ 4: a(n) = 2*(n-1)*a(n-1) -
 *            (n^2-3n+1)*a(n-2) (recurrence from OEIS)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of terms, 1‑based */
static idx_t cap   = 0;          /* allocated capacity of cache   */
static idx_t last  = 0;          /* largest index already computed */

/* make sure cache contains terms up to index n */
static void ensure(idx_t n)
{
    if (n <= last)          /* nothing to do */
        return;

    /* grow array if needed */
    if ((idx_t)(n + 1) > cap) {
        idx_t newcap = (n + 1) * 2;      /* over‑allocate a bit */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);               /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the first three values the first time we need them */
    if (last < 1) cache[1] = (seq_t)1;
    if (last < 2) cache[2] = (seq_t)0;
    if (last < 3) cache[3] = (seq_t)1;

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i < 4) continue;          /* already set */

        /* term1 = 2*(i-1)*a(i-1) */
        seq_t term1 = (seq_t)2 * (seq_t)(i - 1) * cache[i - 1];

        /* term2 = (i^2 - 3*i + 1) * a(i-2) */
        idx_t   coef = i * i - 3 * i + 1;  /* safe for i up to many thousands */
        seq_t   term2 = (seq_t)coef * cache[i - 2];

        cache[i] = term1 - term2;
    }

    last = n;
}

/* external linkage required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined, but return 0 safely */
    ensure(n);
    return cache[n];
}
