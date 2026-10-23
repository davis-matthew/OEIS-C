/*
 * A197131  Sum of the reflection (absolute) lengths of all elements in the
 *          Coxeter group of type D_n.
 * https://oeis.org/A197131
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |W(D_n)| * (∑_{i=1}^{n-1} (2i-1)/(2i) + (n-1)/n) where
 *            |W(D_n)| = 2^(n-1) * n! (the order of the Coxeter group D_n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, calloc, exit */

static seq_t *a_cache = NULL;   /* cache of a(n)   */
static seq_t *f_cache = NULL;   /* cache of |W(D_n)| = 2^(n-1)*n!  */
static idx_t  max_cached = 0;   /* largest n that has been cached   */

/* Extend cached tables up to n (inclusive) */
static void extend_cache(idx_t n)
{
    if (n <= max_cached) return;                           /* already cached */

    size_t new_size = (size_t)n + 1;                        /* indices 0..n */

    if (a_cache == NULL) {                                  /* first time */
        a_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        f_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!a_cache || !f_cache) exit(1);

        /* base values for n = 2 */
        a_cache[2] = (seq_t)4;         /* a(2) = 4               */
        f_cache[2] = (seq_t)4;         /* |W(D_2)| = 4            */
        max_cached = 2;
    } else {
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        f_cache = (seq_t *)realloc(f_cache, new_size * sizeof(seq_t));
        if (!a_cache || !f_cache) exit(1);
    }

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* |W(D_n)| = |W(D_{n-1})| * 2 * n */
        f_cache[i] = f_cache[i-1] * (seq_t)(2 * i);

        seq_t fi = f_cache[i];

        /* term1 = fi / (2*i-2) * (2*i-3)  */
        seq_t t1 = fi / (seq_t)(2 * i - 2);
        t1 = t1 * (seq_t)(2 * i - 3);

        /* term2 = fi / i * (i-1)          */
        seq_t t2 = fi / (seq_t)i;
        t2 = t2 * (seq_t)(i - 1);

        /* term3 = fi / (i-1) * (i-2)      */
        seq_t t3 = fi / (seq_t)(i - 1);
        t3 = t3 * (seq_t)(i - 2);

        /* a(n) = 2*n*a(n-1) + t1 + t2 - t3 */
        seq_t mult = (seq_t)2 * (seq_t)i;
        a_cache[i] = mult * a_cache[i-1] + t1 + t2 - t3;
    }

    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for n<2, return 0 by convention */
    extend_cache(n);
    return a_cache[n];
}
