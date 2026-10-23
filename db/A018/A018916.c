/*
 * A018916  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(2,8).
 * https://oeis.org/A018916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-3); initial terms: a(0)=2, a(1)=8,
 *            a(2)=31
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current capacity of cache array   */
    static idx_t len   = 0;   /* number of values stored in cache  */

    /* initialise cache if first call */
    if (len == 0) {
        /* ensure at least the first three elements exist     */
        cap = (n < 3 ? 3 : n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;           /* out of memory */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)31;
        len = 3;
    }

    /* fast return for already computed indices */
    if (n < len)
        return cache[n];

    /* grow cache if needed */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = 4 * cache[i-1] - 2 * cache[i-3];
    }
    len = n + 1;
    return cache[n];
}
