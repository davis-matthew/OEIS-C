/*
 * A163214  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*(a(n-1)+a(n-2)+a(n-3)-15*a(n-4)) * with special case
 *            a(4) = 29*(a3+a2+a1-15*a0) + 1 * Bases: a(0)=1, a(1)=31,
 *            a(2)=930, a(3)=27900
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;            /* number of computed entries */

    if (n < cache_len)
        return cache[n];

    /* enlarge cache to hold entries up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                           /* memory allocation error */

    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)31;
        } else if (i == 2) {
            cache[i] = (seq_t)930;
        } else if (i == 3) {
            cache[i] = (seq_t)27900;
        } else {
            /* common recurrence part */
            seq_t tmp = cache[i-1] + cache[i-2] + cache[i-3];
            tmp -= (seq_t)15 * cache[i-4];
            tmp = (seq_t)29 * tmp;
            if (i == 4)                 /* special extra 1 for a(4) */
                tmp += (seq_t)1;
            cache[i] = tmp;
        }
    }

    cache_len = new_len;
    return cache[n];
}
