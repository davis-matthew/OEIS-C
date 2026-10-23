/*
 * A167291  a(n) = A137505(2n) + A137505(2n+1).
 * https://oeis.org/A167291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 4*a(n-2) + 4*a(n-3) with initial values a(0)=2,
 *            a(1)=2, a(2)=0
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 65, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated elements (capacity) */
    static idx_t   used   = 0;   /* last computed index + 1 */

    /* ----- initialise base values ----- */
    if (used == 0) {
        cap  = 8;               /* small start capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory, return 0 */
        cache[0] = (seq_t)2;          /* a(0) */
        cache[1] = (seq_t)2;          /* a(1) */
        cache[2] = (seq_t)0;          /* a(2) */
        used = 3;
    }

    /* ----- ensure capacity for index n ----- */
    if ((idx_t)n + 1 > (idx_t)cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while ((idx_t)new_cap <= (idx_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* ----- compute missing values using the recurrence ----- */
    for (idx_t i = used; i <= n; ++i) {
        seq_t val;
        if (i == 0)      val = (seq_t)2;      /* a(0) */
        else if (i == 1) val = (seq_t)2;      /* a(1) */
        else if (i == 2) val = (seq_t)0;      /* a(2) */
        else {
            /* a(i) = a(i-1) - 4*a(i-2) + 4*a(i-3) */
            val = cache[i-1] - (seq_t)4 * cache[i-2] + (seq_t)4 * cache[i-3];
        }
        cache[i] = val;
    }

    used = n + 1;  /* update number of computed elements */
    return cache[n];
}
