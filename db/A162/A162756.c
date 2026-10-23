/*
 * A162756  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2) – 36*a(n-3) for n >= 4 with initial
 *            values a(0)=1, a(1)=10, a(2)=90, a(3)=765
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t   cache_len = 0;  /* number of elements stored */

    /* initialise cache with the first four terms if not yet done */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)90;
        cache[3] = (seq_t)765;
    }

    /* if already computed, return immediately */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache to hold indices up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* allocation failure */
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* for i >= 4 the recurrence applies; for i == 3 the value is already known */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t next = 8 * a1 + 8 * a2 - 36 * a3;   /* all operations are on seq_t */
        cache[i] = next;
    }

    cache_len = new_len;
    return cache[n];
}
