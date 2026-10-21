/*
 * A206351  a(n) = 7*a(n-1) - a(n-2) - 4 with a(1)=1, a(2)=3.
 * https://oeis.org/A206351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - a(n-2) - 4, a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 stores a(1), index 1 stores a(2) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;           /* allocated size (number of terms) */
    size_t  need, i;

    if (n == 0) return 0;                /* undefined, return 0 */

    /* init cache with first two terms */
    if (!cache) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[0] = (seq_t)1;             /* a(1) */
        cache[1] = (seq_t)3;             /* a(2) */
    }

    /* enlarge cache if necessary */
    need = (size_t)(n - 1);              /* array index for a(n) */
    if (need >= cap) {
        size_t new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failed */
        cache = tmp;
        size_t cur = cap;
        /* compute new terms using the recurrence */
        for (i = cur; i < new_cap; ++i) {
            /* cache[i-1] = a(i+1)  (since i is 0‑based) */
            cache[i] = (seq_t)7 * cache[i-1] - cache[i-2] - (seq_t)4;
        }
        cap = new_cap;
    }

    return cache[need];
}
