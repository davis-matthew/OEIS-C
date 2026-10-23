/*
 * A042747  Denominators of continued fraction convergents to sqrt(904).
 * https://oeis.org/A042747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=15, a(2)=901, a(3)=13530,
   a(n)=902*a(n-2)-a(n-4) for n≥4 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* number of allocated slots */
    static idx_t   next = 0;      /* next index that must be computed */

    /* initialise cache if first call */
    if (cache == NULL) {
        capacity = 4;
        if ((idx_t)capacity <= n) {
            capacity = (size_t)n + 1;
        }
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* cannot allocate */

        /* seed the first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)901;
        cache[3] = (seq_t)13530;
        next = 4;
    }
    /* grow cache if required */
    else if (capacity <= (size_t)n) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* cannot realloc */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= next) {
        for (idx_t k = next; k <= n; ++k) {
            if (k == 0) {
                cache[k] = (seq_t)1;
            } else if (k == 1) {
                cache[k] = (seq_t)15;
            } else if (k == 2) {
                cache[k] = (seq_t)901;
            } else if (k == 3) {
                cache[k] = (seq_t)13530;
            } else {
                /* recurrence: a(n) = 902*a(n-2) - a(n-4) */
                cache[k] = (seq_t)902 * cache[k - 2] - cache[k - 4];
            }
        }
        next = n + 1;
    }

    return cache[n];
}
