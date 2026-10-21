/*
 * A238906  Number of (n+1) X (1+1) 0..2 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A238906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) - 3*a(n-3) with a(1)=3, a(2)=9,
 *            a(3)=54 (OEIS A238906)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static 1‑based cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* highest index computed so far */

    if (n == 0)
        return (seq_t)0;             /* undefined for offset 1, but safe */

    /* Extend cache if needed */
    if (n > cap)
    {
        size_t new_cap = (size_t)n + 1;   /* +1 to ignore 0‑index */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Compute values up to n (recompute earlier ones if needed) */
        for (idx_t i = 1; i <= n; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)3;
            else if (i == 2)
                cache[i] = (seq_t)9;
            else if (i == 3)
                cache[i] = (seq_t)54;
            else
                cache[i] = (seq_t)((seq_t)4 * cache[i - 1]
                                 + (seq_t)6 * cache[i - 2]
                                 - (seq_t)3 * cache[i - 3]);
        }
        cap = (size_t)n;
    }

    return cache[n];
}
