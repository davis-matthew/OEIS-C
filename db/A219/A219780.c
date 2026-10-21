/*
 * A219780  1 = Sum_{n>=0} a(n) * x^n * (1 - (n+1)*x)^4.
 * https://oeis.org/A219780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=4*n*a(n-1)-6*(n-1)^2*a(n-2)+4*(n-2)^3*a(n-3)-(n-3)^4*a(n-4),
 *            with base values a(0)=1, a(1)=4, a(2)=26, a(3)=220
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;        /* array of computed terms */
static size_t  cache_len = 0;      /* number of terms currently stored */

/* compute a(n) with memoization */
seq_t a(idx_t n)
{
    /* initialise cache with first four values on first call */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;            /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)26;
        cache[3] = (seq_t)220;
    }

    /* already computed? */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* ensure the backing array is large enough for indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return 0;              /* allocation failed */
        cache = tmp;
    }

    /* start computing from the first unchecked index */
    size_t start = cache_len;    /* value before possible reallocation */

    for (idx_t i = (idx_t)start; i <= n; ++i) {
        /* term1 = 4 * i * a[i-1] */
        seq_t term1 = (seq_t)4 * (seq_t)i;
        term1 *= cache[i - 1];

        /* term2 = 6 * (i-1)^2 * a[i-2] */
        seq_t n1 = (seq_t)(i - 1);
        seq_t term2 = (seq_t)6 * n1 * n1;
        term2 *= cache[i - 2];

        /* term3 = 4 * (i-2)^3 * a[i-3] */
        seq_t n2 = (seq_t)(i - 2);
        seq_t term3 = (seq_t)4 * n2 * n2 * n2;
        term3 *= cache[i - 3];

        /* term4 = (i-3)^4 * a[i-4] */
        seq_t n3 = (seq_t)(i - 3);
        seq_t term4 = n3 * n3 * n3 * n3;
        term4 *= cache[i - 4];

        /* Combine: a[i] = term1 - term2 + term3 - term4  */
        seq_t val = term1 + term3;      /* first add pair */
        seq_t sub = term2 + term4;      /* second add pair */
        cache[i] = val - sub;           /* final subtraction (wraps OK) */
    }

    /* update length after filling up to n */
    cache_len = needed;
    return cache[n];
}
