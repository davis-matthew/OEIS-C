/*
 * A013341  E.g.f.: exp(sin(x)-log(x+1)).
 * https://oeis.org/A013341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A013341:
 *   a(n) are the coefficients of the exponential generating function
 *   exp(sin(x)-log(1+x)) = Σ a(n)/n! · x^n.
 *   Recurrence: a(0)=1; for n≥0
 *     a(n+1)= Σ_{j=0..⌊n/2⌋} (-1)^j·C(n,2j)·a(n-2j)
 *             – Σ_{m=0..n}   (-1)^m·(n)_m·a(n-m) ,
 *   where (n)_m = n·(n-1)…(n-m+1) is the falling factorial.
 *   All arithmetic is performed with the type seq_t; the function
 *   is memoised so that any order of calls for n≥0 is handled.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;          /* number of stored terms */

    /* Initialise with a(0) */
    if (computed == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        computed = 1;
    }

    /* Compute terms up to n if necessary */
    while (computed <= n) {
        idx_t cur = computed - 1;        /* last computed index */

        /* ---------- term1: Σ (-1)^j · C(cur, 2j) · a(cur-2j) ---------- */
        seq_t term1 = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(cur,0) */
        int  sign  = 1;                  /* (-1)^j   */
        idx_t maxj = cur / 2;
        for (idx_t j = 0; j <= maxj; ++j) {
            if (sign > 0)
                term1 += cache[cur - 2 * j] * binom;
            else
                term1 -= cache[cur - 2 * j] * binom;

            if (j < maxj) {
                /* compute C(cur, 2(j+1)) from C(cur, 2j) */
                seq_t n1 = (seq_t)(cur - 2 * j);
                seq_t n2 = (seq_t)(cur - 2 * j - 1);
                seq_t d1 = (seq_t)(2 * j + 1);
                seq_t d2 = (seq_t)(2 * j + 2);
                binom = binom * n1 * n2 / (d1 * d2);
            }
            sign = -sign;                /* toggle sign */
        }

        /* ---------- term2: Σ (-1)^m · (cur)_m · a(cur-m) ---------- */
        seq_t term2 = (seq_t)0;
        seq_t falling = (seq_t)1;        /* (cur)_0   */
        int  sign2   = 1;                /* (-1)^m    */
        for (idx_t m = 0; m <= cur; ++m) {
            if (sign2 > 0)
                term2 += cache[cur - m] * falling;
            else
                term2 -= cache[cur - m] * falling;

            if (m < cur)
                falling *= (seq_t)(cur - m);
            sign2 = -sign2;              /* toggle sign */
        }

        /* a(cur+1) = term1 – term2 */
        seq_t next = term1 - term2;

        /* Grow the cache to accommodate the new term */
        size_t newsize = (size_t)(computed + 1);    /* space for index computed */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache[computed] = next;           /* store a(cur+1) */
        ++computed;
    }

    return cache[n];
}
