/*
 * A098533  Sum of seventh powers of first n Fibonacci numbers.
 * https://oeis.org/A098533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} F(k)^7 (a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;          /* cache[a] holds a(n)  */
    static size_t cache_cap = 0;         /* allocated capacity   */
    static idx_t  max_n    = 0;          /* highest n computed   */
    static seq_t  f_prev   = 0;          /* F(n-1) in Fibonacci seq  */
    static seq_t  f_curr   = 1;          /* F(n)   in Fibonacci seq  */

    /* ensure capacity for indices 0..n */
    if (cache == NULL) {
        /* first call */
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0;                    /* a(0) */
        max_n   = 0;
        f_prev  = 0;                     /* F(0)  */
        f_curr  = 1;                     /* F(1)  */
    } else if (cache_cap <= (size_t)n) {
        /* grow cache */
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return result */
    if (n <= max_n)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* compute F(k)^7 */
        seq_t pow = f_curr;
        int i;
        for (i = 1; i < 7; ++i)
            pow *= f_curr;

        /* a(k) = a(k-1) + F(k)^7 */
        cache[k] = cache[k - 1] + pow;

        /* advance Fibonacci numbers: F(k+1) = F(k-1)+F(k) */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    max_n = n;
    return cache[n];
}
