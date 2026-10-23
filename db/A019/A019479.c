/*
 * A019479  Define the sequence S(a_0,a_1) by a_{n+2} is the least integer
 *          such that a_{n+2}/a_{n+1}>a_{n+1}/a_n for n >= 0. This is S(4,8).
 * https://oeis.org/A019479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) - a(n-4) (n >= 4) Initial
 *            values: a(0)=4, a(1)=8, a(2)=17, a(3)=37
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;    /* array of terms */
    static size_t  cache_cap = 0;    /* allocated capacity */
    static idx_t   cache_last = (idx_t)-1; /* last computed index */

    /* initialise first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);                     /* allocation failed */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)37;
        cache_last = 3;
    }

    /* return if already computed */
    if (n <= cache_last) return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (newp == NULL) exit(1);               /* allocation failed */
        cache = newp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3] - cache[i-4];
    }
    cache_last = n;
    return cache[n];
}
