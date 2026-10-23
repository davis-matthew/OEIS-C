/*
 * A049391  Expansion of (1-25*x)^(3/5).
 * https://oeis.org/A049391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (5^n/n!) * ∏_{k=0}^{n-1} (5k-3) Recurrence: a(0)=1;
 *            a(n)=a(n-1)*(5*(5n-8))/n for n≥1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* External linkage function requested by the harness */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements already computed */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (cache == NULL)            /* first call: initialise cache[0] = 1 */
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache_size = 1;
    }

    /* grow capacity if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute up to requested n */
    while (cache_size <= n)
    {
        idx_t i = cache_size;                         /* next index to calculate */
        seq_t prev = cache[i - 1];                    /* a(i-1) */
        seq_t ii   = (seq_t)i;                       /* cast index to seq_t */
        seq_t numerator = 5 * (5 * ii - 8);           /* 5*(5i-8) */

        /* cancel common factors between prev and i */
        seq_t g = gcd(prev, ii);
        prev /= g;
        seq_t n_div = ii / g;

        /* cancel any remaining common factors between prev and n_div */
        g = gcd(prev, n_div);
        prev /= g;
        n_div /= g;

        /* cancel any common factors between numerator and the remaining n_div */
        g = gcd(numerator, n_div);
        numerator /= g;
        n_div /= g;   /* should be 1 for all valid n */

        cache[i] = prev * numerator;
        ++cache_size;
    }

    return cache[n];
}
