/*
 * A067010  a(2n) and a(2n+1) are side lengths of a Beentjes sequence of
 *          perfect squared rectangles, starting with a 32 X 33 rectangle.
 * https://oeis.org/A067010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=32, a(1)=33, a(2)=682, a(3)=779, a(n)=22*a(n-2)+9*a(n-4)
 *            for n>3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  used    = 0;   /* number of values stored       */
    static idx_t  capacity = 0;  /* allocated capacity of cache   */

    /* Initialise cache with the first four terms */
    if (used == 0) {
        capacity = 16;                    /* starting capacity > 4          */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);

        cache[0] = (seq_t)32;
        cache[1] = (seq_t)33;
        cache[2] = (seq_t)682;
        cache[3] = (seq_t)779;
        used = 4;
    }

    /* Grow capacity if the requested index is beyond it */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = used; i <= n; ++i) {
        if (i < 4) continue;                     /* already known */
        cache[i] = (seq_t)22 * cache[i-2] + (seq_t)9 * cache[i-4];
    }
    used = n + 1;                                 /* we now have [0..n] */

    return cache[n];
}
