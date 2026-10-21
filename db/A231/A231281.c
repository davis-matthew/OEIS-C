/*
 * A231281  Number of n X 4 0..3 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A231281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 86*a(n-2) + 190*a(n-3) - 29*a(n-4) + a(n-5) (n
 *            ≥ 6) Initial values: a(1)=0, a(2)=6, a(3)=76, a(4)=1170,
 *            a(5)=18208 The function caches computed terms so that subsequent
 *            calls (in any order) are O(1). It works for any seq_t type
 *            (built‑in or arbitrary‑precision)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset starts at 1; return 0 for undefined 0 */
    if (n == 0) return 0;

    /* static cache of already computed terms, one per n */
    static seq_t *cache = NULL;
    static size_t cap   = 0;            /* number of allocated slots */

    size_t idx = (size_t)n;             /* number of terms that must be present */

    if (cap < idx) {                    /* grow cache to the required size */
        /* use a cast to satisfy C++'s stricter type rules */
        seq_t *tmp = (seq_t *)realloc(cache, idx * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure → safe fallback */
        cache = tmp;

        /* compute new entries */
        for (size_t i = cap; i < idx; ++i) {
            if (i == 0)         cache[0] = 0;          /* a(1) */
            else if (i == 1)    cache[1] = 6;          /* a(2) */
            else if (i == 2)    cache[2] = 76;         /* a(3) */
            else if (i == 3)    cache[3] = 1170;       /* a(4) */
            else if (i == 4)    cache[4] = 18208;      /* a(5) */
            else /* recurrence for n ≥ 6 */
                cache[i] = cache[i-1]*9 + cache[i-2]*86
                         + cache[i-3]*190 - cache[i-4]*29
                         + cache[i-5];
        }
        cap = idx;
    }
    return cache[n-1];   /* cache[0] holds a(1) */
}
