/*
 * A052886  Expansion of e.g.f.: (1/2) - (1/2)*(5 - 4*exp(x))^(1/2).
 * https://oeis.org/A052886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + ∑_{k=1}^{n-1} binomial(n,k)*a(k)*a(n-k) a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static cache holds values that have already been computed */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;
    static idx_t  last       = 0;   /* highest n computed so far */

    /* lazy initialization: store a(0) and a(1) */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 0;
        cache[1] = 1;
        last    = 1;
    }

    /* return cached value if it is already known */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to accommodate indices up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_size);
    if (!tmp)          /* allocation failed – return 0 as error */
        return 0;
    cache = tmp;
    cache_size = new_size;

    /* compute missing values from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* use symmetry to halve the number of binomial evaluations */
        idx_t half = (i - 1) / 2;   /* floor((i-1)/2) */

        for (idx_t k = 1; k <= half; ++k) {
            seq_t t = binom(i, k);          /* binomial(i,k) */
            t *= cache[k];
            t *= cache[i - k];
            sum += t;
            sum += t;                       /* symmetric counterpart */
        }

        /* for even i, add the middle term once */
        if ((i & 1) == 0) {
            idx_t mid = i / 2;
            seq_t t = binom(i, mid);
            t *= cache[mid];
            t *= cache[mid];
            sum += t;
        }

        cache[i] = sum + 1;   /* recurrence: a(i) = 1 + sum */
    }

    last = n;
    return cache[n];
}
