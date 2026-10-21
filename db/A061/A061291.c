/*
 * A061291  a(1) = 1; a(n+1) = a(1) + a(2)*(a(2) + a(3)*(a(3)
 *          +...+a(n-1)*(a(n-1) + a(n))...)).
 * https://oeis.org/A061291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Nested-recursive sequence A061291
 *
 *  a(1) = 1;  a(2) = 1.
 *  For n >= 2:
 *      a(n+1) = 1 + T
 *  where T is computed from the previous terms as
 *      T = a(n)
 *      for i = n-1 down to 2:
 *          T = a(i) * (a(i) + T)
 *
 *  This implements the nesting
 *      a(1) + a(2)*(a(2)+a(3)*(a(3)+ … + a(n-1)*(a(n-1)+a(n))…)).
 */

#include <stdlib.h>

/* Computes a(n) for n >= 1.
 *  The sequence is memoised statically so that calls with arbitrary n
 *  do not recompute earlier terms.
 */
seq_t a(idx_t n)
{
    /* static cache: index 1..cached are already computed */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* largest n computed   */
    static size_t cap = 0;     /* allocated slots      */

    if (n < 1)
        return 0;              /* undefined, treat as 0 */

    /* initialise the cache if needed */
    if (cache == NULL) {
        cap = 4;                /* at least store up to index 3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;          /* allocation failed */

        cache[1] = 1;
        cache[2] = 1;
        cached = 2;
    }

    /* grow cache if needed */
    if (n > cached) {
        size_t needed = (size_t)n + 1;   /* indices are 1-based */
        if (needed > cap) {
            size_t newcap = cap;
            while (newcap < needed)
                newcap = newcap < 8 ? 8 : newcap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL)
                return 0;      /* allocation failed */
            cache = tmp;
            cap = newcap;
        }

        /* compute missing terms up to n */
        for (idx_t k = cached + 1; k <= n; ++k) {
            if (k == 2) {               /* defined base case */
                cache[2] = 1;
                continue;
            }

            seq_t t = cache[k - 1];      /* start with a(k-1) = a(n)   */
            /* descending loop from k-2 down to 2 */
            if (k > 3) {                /* loop needed only when k-2 >= 2 */
                for (idx_t i = k - 2; ; ) {
                    t = cache[i] * (cache[i] + t);
                    if (i == 2) break;
                    --i;
                }
            }
            cache[k] = cache[1] + t;    /* a(k) = 1 + t, as a(1)=1 */
        }

        cached = n;
    }

    return cache[n];
}
