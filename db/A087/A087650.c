/*
 * A087650  a(n) = Sum_{k=0..n} (-1)^(n-k)*Bell(k).
 * https://oeis.org/A087650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} binom(n,k) * a(k-1), a(0)=1, a(1)=0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cap = 0;               /* allocated capacity */
    static idx_t computed = (idx_t)-1;  /* highest n that has been computed */

    /* ensure the cache holds at least n+1 entries */
    if (cap < n + 1) {
        idx_t newcap = (cap > 1) ? cap * 2 : 2;
        while (newcap < n + 1)
            newcap *= 2;

        cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        cap = newcap;
    }

    /* first call: initialise a(0) and a(1) */
    if (computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (n >= 1)
            cache[1] = (seq_t)0;
        computed = 0;   /* a(0) is now available */
    }

    /* if a(n) already known, return it */
    if (n <= computed)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = (seq_t)i;   /* binom(i,1) */

        for (idx_t k = 1; k < i; ++k) {
            sum += binom * cache[k - 1];   /* add term binom(i,k) * a(k-1) */

            if (k < i - 1) {              /* prepare next binomial coefficient */
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }

        cache[i] = sum;
    }

    computed = n;
    return cache[n];
}
