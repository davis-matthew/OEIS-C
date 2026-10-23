/*
 * A037603  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,3,0.
 * https://oeis.org/A037603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037603
   Formula: a(n) = floor(13 * 10^(n+1) / 999)
   Recurrence: a(n) = 10 * a(n-1) + a(n-3) - 10 * a(n-4)
   Initial terms: a(1)=1, a(2)=13, a(3)=130, a(4)=1301
   (Indices start at 1)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache previously computed terms.
       The cache holds a(i) in cache[i-1]. */
    static seq_t *cache = NULL;
    static idx_t max_n  = 0;   /* number of terms currently stored */

    if (n == 0)                 /* defined only for n ≥ 1; behaviour below offset is irrelevant */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if (n <= max_n)
        return cache[n - 1];

    /* Resize the cache to hold up to n terms. */
    cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!cache) return (seq_t)0;  /* allocation failed – return 0 (sequence undefined) */

    /* Compute any new terms that are not yet in the cache. */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)13;
        else if (i == 3)
            val = (seq_t)130;
        else if (i == 4)
            val = (seq_t)1301;
        else
            /* a(i) = 10*a(i-1) + a(i-3) - 10*a(i-4)   */
            val = cache[i - 2] * (seq_t)10
                 + cache[i - 4]
                 - cache[i - 5] * (seq_t)10;
        cache[i - 1] = val;
    }

    max_n = n;
    return cache[n - 1];
}
