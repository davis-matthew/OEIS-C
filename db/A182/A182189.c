/*
 * A182189  a(n) = 6*a(n-1) - a(n-2) - 4 with n > 1, a(0)=1, a(1)=3.
 * https://oeis.org/A182189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 4 ; a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  last  = 1;   /* last index computed */

    /* initial allocation and base values */
    if (!cache) {
        cap = (n + 1 > 2) ? n + 1 : 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        last = 1;
    }
    /* grow cache if needed */
    else if (n + 1 > cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cap = n + 1;
    }

    /* return already known value */
    if (n <= last)
        return cache[n];

    /* compute new values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = 6 * cache[i-1] - cache[i-2] - (seq_t)4;

    last = n;
    return cache[n];
}
