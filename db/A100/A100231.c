/*
 * A100231  G.f. A(x) satisfies: 5^n - 1 = Sum_{k=0..n} [x^k]A(x)^n and also
 *          satisfies: (5+z)^n - (1+z)^n + z^n = Sum_{k=0..n}
 *          [x^k](A(x)+z*x)^n for all z, where [x^k]A(x)^n denotes the
 *          coefficient of x^k in A(x)^n.
 * https://oeis.org/A100231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -((4*n-6)*a(n-1)+20*(n-3)*a(n-2))/n for n>2, with a(0)=1,
 *            a(1)=3, a(2)=4
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to handle arbitrary call order and reuse */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */

    /* Initialise cache with first three values */
    if (cache == NULL) {
        /* Allocate space for indices 0,1,2 */
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache_size = 3;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
    }

    /* If requested index is already cached, return it immediately */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Need to compute values up to n inclusive */
    size_t target = (size_t)n;          /* n is assumed ≤ 10000 */
    if (target + 1 > cache_size) {
        /* Extend cache */
        seq_t *tmp = (seq_t*)realloc(cache, (target + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();       /* allocation failure */
        cache = tmp;
    }

    /* Compute missing entries sequentially */
    for (size_t i = cache_size; i <= target; ++i) {
        /* i is the current index to compute */
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)3;
        } else if (i == 2) {
            cache[i] = (seq_t)4;
        } else {
            /* Using seq_t for all arithmetic to preserve precision */
            seq_t coeff1 = (seq_t)4 * (seq_t)i - (seq_t)6;          /* 4*i-6 */
            seq_t coeff2 = (seq_t)20 * (seq_t)(i - 3);             /* 20*(i-3) */
            seq_t num     = coeff1 * cache[i - 1] + coeff2 * cache[i - 2];
            cache[i] = -(num / (seq_t)i);                          /* integer division */
        }
    }

    cache_size = target + 1;
    return cache[n];
}
