/*
 * A100228  G.f. A(x) satisfies: 4^n - 1 = Sum_{k=0..n} [x^k] A(x)^n and also
 *          satisfies: (4+z)^n - (1+z)^n + z^n = Sum_{k=0..n}
 *          [x^k](A(x)+z*x)^n for all z, where [x^k] A(x)^n denotes the
 *          coefficient of x^k in A(x)^n.
 * https://oeis.org/A100228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -( (2*n-3)*a(n-1) + 13*(n-3)*a(n-2) )/n for n>2 * a(0)=1,
 *            a(1)=2, a(2)=3
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 18, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised.  It works for both built‑in integral
 * types and for arbitrary‑precision integer classes as long as
 * the class supports the operators +, -, *, / and a constructor
 * from integer constants. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* current number of cached entries */

    /* initialise cache with the first three terms (a(0)=1, a(1)=2, a(2)=3) */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
    }

    /* if the value is already cached, return it immediately */
    if ((idx_t)n < cache_len)
        return cache[n];

    /* enlarge the cache to hold the requested index */
    const size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* reallocation failed */
    cache = tmp;
    size_t old_len = cache_len;
    cache_len = new_len;

    /* compute missing terms using the recurrence */
    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        if (i == 1) { cache[1] = (seq_t)2; continue; }
        if (i == 2) { cache[2] = (seq_t)3; continue; }

        /* coefficients of the recurrence */
        seq_t coeff1 = (seq_t)2 * (seq_t)i - (seq_t)3;         /* 2*n - 3 */
        seq_t coeff2 = (seq_t)13 * ((seq_t)i - (seq_t)3);      /* 13*(n-3) */

        /* numerator: -(coeff1*a[n-1] + coeff2*a[n-2]) */
        seq_t temp = coeff1 * cache[i - 1] + coeff2 * cache[i - 2];
        temp = temp * (seq_t)(-1);

        /* divide by n (exact division) */
        cache[i] = temp / (seq_t)i;
    }

    return cache[n];
}
