/*
 * A028393  Iterate the map in A006368 starting at 8.
 * https://oeis.org/A028393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 8. For n ≥ 1, a(n) = f(a(n‑1)) where the map f is defined
 *            as in A006368: if x is even: f(x) = x + x/2 (i.e. 3x/2) if x is
 *            odd and x % 4 == 1: f(x) = x – x/4 if x is odd and x % 4 == 3:
 *            f(x) = x – (x/4 + 1)
 * Range:     u8 n <= 18, u16 n <= 129, u32 n <= 394, u64 n <= 844,
 *            u128 n <= 1726, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialisation of cache[0] = 8 */
    if (cache_len == 0) {
        cache_cap = 64;                     /* initial capacity */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed: return 0 */
        cache_len  = 1;
        cache[0]   = (seq_t)8;
    }

    /* if requested index already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* ensure capacity for indices up to n */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap;
        while ((idx_t)(n + 1) > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;               /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute successive terms from cache_len to n */
    seq_t cur = cache[cache_len - 1];
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t next;
        if ((cur & (seq_t)1) == 0) {          /* even case */
            next = cur + cur / 2;             /* f(x) = 3x/2 */
        } else {                              /* odd case */
            seq_t q = cur / 4;                /* floor(cur/4) */
            seq_t r = cur % 4;                /* cur % 4  (1 or 3) */
            if (r == 1)
                next = cur - q;
            else                              /* r == 3 */
                next = cur - (q + (seq_t)1);
        }
        cache[i] = next;
        cur = next;
    }

    cache_len = n + 1;
    return cache[n];
}
