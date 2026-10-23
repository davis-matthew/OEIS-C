/*
 * A129625  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+233)^2 = y^2.
 * https://oeis.org/A129625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 466 for n > 6; a(1)=0, a(2)=75,
 *            a(3)=432, a(4)=699, a(5)=1092, a(6)=3115
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 68, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* array of computed terms */
    static idx_t  cap   = 0;           /* current allocated capacity (≥ n+1) */
    static idx_t  max_n = 0;           /* largest index that has been computed */

    /* initial allocation and seed with the known first six terms */
    if (cache == NULL) {
        cap = 7;                       /* indices 0..6 needed */
#ifdef __cplusplus
        cache = new seq_t[cap];
#else
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
#endif
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)75;
        cache[3] = (seq_t)432;
        cache[4] = (seq_t)699;
        cache[5] = (seq_t)1092;
        cache[6] = (seq_t)3115;
        max_n   = 6;
    }

    /* grow the cache if the requested index is beyond current capacity */
    if (n >= cap) {
        idx_t new_cap = n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cap = new_cap;
    }

    /* return a value that was already computed */
    if (n <= max_n)
        return cache[n];

    /* compute all missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t v = cache[i - 3] * (seq_t)6;
        v = v - cache[i - 6];
        v = v + (seq_t)466;
        cache[i] = v;
    }
    max_n = n;
    return cache[n];
}
