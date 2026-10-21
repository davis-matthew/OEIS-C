/*
 * A228920  Number of solutions to Sum_{i=1..n} x_i^2 == 0 (mod 4) with x_i in
 *          0..3.
 * https://oeis.org/A228920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2+2i)^n + (2-2i)^n + 4^n)/4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=4, a(3)=8,
 *             a(n)=8*a(n-1)-24*a(n-2)+32*a(n-3)  for n>=4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms stored in cache (max index) */
    static size_t cap = 0;     /* allocated capacity for cache */

    if (n == 0)
        return 0;   /* not used, but safe return */

    /* reserve at least three base terms */
    if (cached == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cap   = 3;
        cache[0] = 2;   /* a(1) */
        cache[1] = 4;   /* a(2) */
        cache[2] = 8;   /* a(3) */
        cached = 3;
    }

    /* grow cache if needed (doubling strategy) */
    if (n > cached) {
        size_t new_cap = cap;
        while ((idx_t)new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* memory allocation failed */
        cache = tmp;
        cap   = new_cap;

        /* compute missing terms up to n */
        for (idx_t i = cached; i < n; ++i) {
            seq_t a1 = cache[i - 1];   /* a(i)  (since i is 0‑based index) */
            seq_t a2 = cache[i - 2];   /* a(i-1) */
            seq_t a3 = cache[i - 3];   /* a(i-2) */
            /* a(i+1) = 8*a1 - 24*a2 + 32*a3 */
            seq_t t = a1 * 8;
            t -= a2 * 24;
            t += a3 * 32;
            cache[i] = t;
        }
        cached = n;
    }

    return cache[n - 1];
}
