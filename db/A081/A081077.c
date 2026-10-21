/*
 * A081077  a(n) = Lucas(4*n+2) + 3, or Lucas(2*n)*Lucas(2*n+2).
 * https://oeis.org/A081077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4*n+2) + 3 Equivalently: a(0)=6, a(1)=21, a(2)=126,
 *            a(n)=8*a(n-1)-8*a(n-2)+a(n-3) for n≥3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* computed a(k) values */
    static idx_t capacity = 0;       /* size of allocated array */
    static idx_t last = 0;           /* index of the largest computed value */

    /* initialise on first call */
    if (cache == NULL) {
        capacity = 3;                     /* store a(0), a(1), a(2) */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)6;   /* a(0) */
        cache[1] = (seq_t)21;  /* a(1) */
        cache[2] = (seq_t)126; /* a(2) */
        last = 2;
    }

    /* grow cache if needed */
    if (capacity <= n) {
        idx_t newcap = capacity;
        while (newcap <= n) newcap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* reallocation failure */
        cache = tmp;
        capacity = newcap;
    }

    /* compute missing terms up to n */
    while (last < n) {
        /* recurrence: a(k+1)=8*a(k)-8*a(k-1)+a(k-2) */
        seq_t term = ((seq_t)8 * cache[last])     /* 8*a(k)   */
                     - ((seq_t)8 * cache[last-1]) /* 8*a(k-1) */
                     + cache[last-2];             /* +a(k-2)  */
        cache[++last] = term;
    }

    return cache[n];
}
