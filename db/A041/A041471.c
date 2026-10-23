/*
 * A041471  Denominators of continued fraction convergents to sqrt(251).
 * https://oeis.org/A041471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for denominators of the continued‑fraction convergents to √251
   a(0) = 1
   a(1) = 1
   For n ≥ 2:
       a(n) = c[(n-1) % 14] * a(n-1) + a(n-2)
   where the partial quotients are
       c = {1,5,2,1,2,2,15,2,2,1,2,5,1,30}  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values up to the largest n seen */
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* coefficients of the periodic part of the continued fraction of √251 */
    const unsigned coef[14] = {1,5,2,1,2,2,15,2,2,1,2,5,1,30};

    /* trivial cases */
    if (n < 2)
        return (seq_t)1;

    /* grow the cache if it does not yet contain a(n) */
    if (cache_len <= n)
    {
        idx_t new_len = n + 1;                           /* store 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)                                      /* allocation failed   */
            return 0;
        cache = tmp;

        /* initialize the first two elements if this is the first use */
        if (cache_len == 0)
        {
            cache[0] = (seq_t)1;
            /* new_len is at least 2 here because n ≥ 2 */
            cache[1] = (seq_t)1;
        }

        /* compute new entries that are not yet cached */
        idx_t start = (cache_len < 2) ? 2 : cache_len;
        for (idx_t i = start; i <= n; ++i)
        {
            unsigned c = coef[(i - 1) % 14];
            cache[i] = (seq_t)c * cache[i - 1] + cache[i - 2];
        }

        cache_len = new_len;
    }

    return cache[n];
}
