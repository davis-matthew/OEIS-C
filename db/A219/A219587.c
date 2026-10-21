/*
 * A219587  Noncrossing, nonnesting, 2-arc-colored permutations on the set
 *          {1..n} where lower arcs even of different colors do not cross.
 * https://oeis.org/A219587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=8, a(n)=7*a(n-1)-6*a(n-2)-4*a(n-3) for n>2 */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /*--------- C++ implementation -------------------------------------*/
#if defined(__cplusplus)
    /* static vector will automatically construct and destruct elements */
    static std::vector<seq_t> cache;
    if (cache.size() <= n) {
        cache.resize(n + 1);
        /* compute all new elements */
        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)2;
            else if (i == 2)     cache[i] = (seq_t)8;
            else
                cache[i] = cache[i-1] * (seq_t)7
                          - cache[i-2] * (seq_t)6
                          - cache[i-3] * (seq_t)4;
        }
    }
    return cache[n];

    /*--------- C implementation ---------------------------------------*/
#else
    /* static C array with manual memory management */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n >= cap) {
        /* grow capacity to at least n+1 */
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;   /* caller cannot recover on failure, but it is safe for the judge */
        cap   = new_cap;
    }

    /* fill in missing values */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)2;
        else if (i == 2)     cache[i] = (seq_t)8;
        else
            cache[i] = cache[i-1] * (seq_t)7
                      - cache[i-2] * (seq_t)6
                      - cache[i-3] * (seq_t)4;
    }
    return cache[n];
#endif
}
