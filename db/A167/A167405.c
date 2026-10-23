/*
 * A167405  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) + 33 * Σ_{k=1}^{min(n,13)} a(n-k) – 561 * a(n-14)
 *            where p(0)=p(14)=1, p(1)=…=p(13)=2 and p(n)=0 for n≥15
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the coefficients of the rational generating function
   (t^14 + 2*t^13 + … + 2*t + 1)/(561*t^14 – 33*t^13 – … – 33*t + 1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *terms = NULL;
    static idx_t terms_cap = 0;   /* current allocated size */
    static idx_t terms_next = 0;  /* number of terms already computed */

    /* allocate initial storage */
    if (terms_cap == 0) {
        terms_cap = 16;              /* at least 16 elements to cover first 15 terms */
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0; /* allocation failure: return zero */
    }

    /* enqueue capacity to accommodate n */
    if (terms_cap <= n) {
        idx_t new_cap = terms_cap;
        while (new_cap <= n) new_cap <<= 1;   /* grow at least double */
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure: return zero */
        terms = tmp;
        terms_cap = new_cap;
    }

    /* compute terms on demand */
    for (idx_t i = terms_next; i <= n; ++i) {
        seq_t p = 0;
        if (i == 0 || i == 14)
            p = (seq_t)1;
        else if (i >= 1 && i <= 13)
            p = (seq_t)2;

        seq_t sum = 0;
        idx_t k;
        /* add 33 * a(i-k) for k = 1 .. min(i,13) */
        for (k = 1; k <= 13 && k <= i; ++k) {
            sum += terms[i - k] * (seq_t)33;
        }
        /* subtract 561 * a(i-14) if applicable */
        if (i >= 14) {
            sum -= terms[i - 14] * (seq_t)561;
        }
        terms[i] = p + sum;
    }
    terms_next = n + 1;   /* memoize that terms up to n are now computed */

    return terms[n];
}
