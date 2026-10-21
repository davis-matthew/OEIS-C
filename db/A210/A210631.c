/*
 * A210631  G.f. for Ehrhart quasi-polynomials for hyperplane arrangements of
 *          type F_4.
 * https://oeis.org/A210631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-2)-a(n-3)-a(n-4)-2*a(n-5)
 *            +2*a(n-7)+a(n-8)+a(n-9)-2*a(n-10)-a(n-11)+a(n-12) Derived from
 *            the generating function x^12/((1-x)(1-x^2)^2(1-x^3)(1-x^4))
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Computes a(n) with memoisation for arbitrary n. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* # of stored terms */
    static size_t terms_cap = 0;   /* capacity of the allocation */

    /* Initialise cache on first call.  The recurrence order is 12, so we
       need the first 13 terms (indices 0..12). */
    if (!terms) {
        terms_cap = 13;
        terms_len = 13;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();

        /* a(0)..a(11) are 0; a(12) = 1. */
        for (size_t i = 0; i < 12; ++i)
            terms[i] = (seq_t)0;
        terms[12] = (seq_t)1;
    }

    /* For n < 12 the sequence is 0. */
    if (n < 12)
        return (seq_t)0;

    /* If we already have the term, return it. */
    if (n < terms_len)
        return terms[n];

    /* Grow the cache if necessary. */
    if (terms_len == terms_cap) {
        terms_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = terms_len; i <= n; ++i) {
        /* We can safely cast i to size_t because the sequence is
           only required for moderate n (e.g. up to 10000). */
        size_t idx = (size_t)i;

        seq_t val = terms[idx-1]                    /* a(n-1) */
                  + terms[idx-2] + terms[idx-2]     /* 2*a(n-2) */
                  - terms[idx-3]                    /* a(n-3) */
                  - terms[idx-4]                    /* a(n-4) */
                  - terms[idx-5] - terms[idx-5]     /* 2*a(n-5) */
                  + terms[idx-7] + terms[idx-7]     /* 2*a(n-7) */
                  + terms[idx-8]                    /* a(n-8) */
                  + terms[idx-9]                    /* a(n-9) */
                  - terms[idx-10] - terms[idx-10]   /* 2*a(n-10) */
                  - terms[idx-11]                   /* a(n-11) */
                  + terms[idx-12];                  /* a(n-12) */

        terms[idx] = val;
    }

    terms_len = (size_t)n + 1;
    return terms[n];
}
