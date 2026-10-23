/*
 * A121673  a(n) = [x^n] (1 + x*(1+x)^(n-1) )^n.
 * https://oeis.org/A121673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * C((n-1)*k, n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;          /* array of seq_t values */
    static idx_t  cached_size = 0;      /* number of entries stored */

    /* Extend cache to store up to n */
    if (n >= cached_size) {
        idx_t new_size = n + 1;
        /* reallocate to accommodate new_size entries */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – return zero for safety */
            return (seq_t)0;
        }
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                /* first binomial C(i,k) */
                seq_t c1 = binom(i, k);
                /* second binomial C((i-1)*k, i-k) */
                idx_t t = (i - 1) * k;
                idx_t r = i - k;
                seq_t c2 = binom(t, r);
                /* multiply and add to sum */
                sum += c1 * c2;
            }
            cache[i] = sum;
        }
        cached_size = new_size;
    }
    return cache[n];
}
