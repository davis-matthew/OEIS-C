/*
 * A020933  Expansion of (1-4*x)^(21/2).
 * https://oeis.org/A020933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-4)^n * binomial(21/2, n)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n≥1: a(n) = ((4*n-46)/n) * a(n-1) */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */

    if (n < cache_len)          /* already computed */
        return cache[n];

    /* Grow cache to hold terms up to n */
    size_t new_len = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (cache == NULL) exit(1);   /* allocation failure */

    /* Compute missing terms */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        seq_t prev = cache[i - 1];

        /* a(i) = ((4*i-46)/i) * prev */
        seq_t num = (seq_t)(4 * (seq_t)i - (seq_t)46);  /* numerator may be negative */
        seq_t den = (seq_t)i;                           /* denominator is positive */

        seq_t sign = 1;
        if (num < 0) {
            sign = -1;
            num = -num;  /* work with absolute values for gcd */
        }

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce prev with remaining denominator */
        seq_t abs_prev = prev < 0 ? -prev : prev;
        seq_t h = gcd(abs_prev, den);
        prev /= h;
        den /= h;

        seq_t prod = prev * num;
        prod *= sign;
        cache[i] = prod / den;
    }

    cache_len = new_len;
    return cache[n];
}
