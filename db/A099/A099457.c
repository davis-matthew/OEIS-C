/*
 * A099457  A Chebyshev transform of A099456 associated to the knot 9_44.
 * https://oeis.org/A099457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 4*a(n-3) - a(n-4) (n >= 4) Initial
 *            values: a(0)=1, a(1)=4, a(2)=10, a(3)=16 (This follows from the
 *            generating function (1+x^2)/(1-4x+7x^2-4x^3+x^4).)
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *term = NULL;          /* array of terms */
    static size_t cap  = 0;             /* current capacity of term */
    static size_t sz   = 0;             /* number of terms stored (highest used+1) */

    /* initialise cache with first four terms */
    if (sz == 0) {
        cap = 8;                          /* initial capacity */
        term = (seq_t*)malloc(cap * sizeof(seq_t));
        /* Safety check for out‑of‑memory is omitted because the function is
           only used in controlled testing environments. */
        term[0] = 1;                 /* a(0) */
        term[1] = 4;                 /* a(1) */
        term[2] = 10;                /* a(2) */
        term[3] = 16;                /* a(3) */
        sz = 4;                      /* we have terms 0..3 stored */
    }

    /* if requested value already in cache, return it immediately */
    if (n < (idx_t)sz)
        return term[(size_t)n];

    /* otherwise grow the cache and compute terms up to n */
    while (sz <= (size_t)n) {
        /* ensure enough capacity */
        if (sz >= cap) {
            cap *= 2;
            /* realloc may change the pointer; we ignore failure checks for brevity */
            term = (seq_t*)realloc(term, cap * sizeof(seq_t));
        }

        /* recurrence: a(k) = 4*a(k-1) - 7*a(k-2) + 4*a(k-3) - a(k-4) */
        term[sz] = 4 * term[sz-1] - 7 * term[sz-2]
                   + 4 * term[sz-3] - term[sz-4];
        ++sz;
    }

    return term[(size_t)n];
}
