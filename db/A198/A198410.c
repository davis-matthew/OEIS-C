/*
 * A198410  a(n) = ((3^(n-1) + 1)^3 - 1)/3^n.
 * https://oeis.org/A198410
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3^(n-1)+1)^3 - 1)/3^n Recurrence (for n>4): a(n) =
 *            13*a(n-1) - 39*a(n-2) + 27*a(n-3) with a(2)=7, a(3)=37, a(4)=271
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* behaviour for indices below the offset (n < 2) is not required */
    if (n < 2)
        return (seq_t)0;

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_max = 1;   /* maximum n for which cache is filled */

    /* grow the cache if necessary */
    if (n > cached_max) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)   /* allocation failed */
            return (seq_t)0;
        cache = tmp;

        /* compute new terms up to n */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i == 2)
                cache[i] = (seq_t)7;
            else if (i == 3)
                cache[i] = (seq_t)37;
            else if (i == 4)
                cache[i] = (seq_t)271;
            else
                cache[i] = (seq_t)13 * cache[i - 1]
                           - (seq_t)39 * cache[i - 2]
                           + (seq_t)27 * cache[i - 3];
        }
        cached_max = n;
    }

    return cache[n];
}
