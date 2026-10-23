/*
 * A105051  Define a(1)=0, a(2)=0, a(3)=15, a(4)=111 then a(n) = 254*a(n-2) -
 *          a(n-4) + 126 also sequence such that 7*a(n)*(a(n) + 1) + 1 = a
 *          square.
 * https://oeis.org/A105051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 254*a(n-2) - a(n-4) + 126 with a(1)=0, a(2)=0, a(3)=15,
 *            a(4)=111
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current allocation size */
    static idx_t size  = 0;   /* number of terms stored (== cap after init) */

    /* ensure the cache is allocated and base terms are present */
    if (cache == NULL) {
        /* start with capacity 4 for initial terms */
        cap  = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* fallback on OOM */
        /* base values */
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)15;  /* a(3) */
        cache[3] = (seq_t)111; /* a(4) */
        size = cap;
    }

    if (n <= size)                  /* already computed */
        return cache[n - 1];

    /* grow cache to hold up to n */
    if (n > cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* fallback on OOM */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 4) {          /* fill any missing base terms */
            if (i == 1) val = (seq_t)0;
            else if (i == 2) val = (seq_t)0;
            else if (i == 3) val = (seq_t)15;
            else             val = (seq_t)111;
        } else {
            /* recurrence: a(i) = 254*a(i-2) - a(i-4) + 126 */
            val = (seq_t)254 * cache[(i - 3)]
                - cache[(i - 5)]
                + (seq_t)126;
        }
        cache[i - 1] = val;
    }
    size = n;
    return cache[n - 1];
}
