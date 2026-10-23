/*
 * A159035  a(0)=1=a(1), a(2)=2, a(3)=5; thereafter
 *          a(n+3)=4*a(n+2)-4*a(n+1)+2*a(n) for n>=1.
 * https://oeis.org/A159035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n+3) = 4*a(n+2) - 4*a(n+1) + 2*a(n) with a(0) = 1, a(1) = 1,
 *            a(2) = 2, a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = (seq_t *)NULL; /* computed terms */
    static size_t cap   = 0;             /* allocated capacity */
    static size_t len   = 0;             /* number of terms computed */

    /* Initialise cache on first call */
    if (!cache) {
        size_t init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure */
        cap  = init_cap;
        len  = 4;                     /* we will store the first 4 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
    }

    /* Ensure capacity for up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 8;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
            if (new_cap < 8) new_cap = 8; /* safety guard */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);             /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    if (len <= (size_t)n) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 4*a(i-1) - 4*a(i-2) + 2*a(i-3) */
            cache[i] = (seq_t)(4 * cache[i - 1]
                               - 4 * cache[i - 2]
                               + 2 * cache[i - 3]);
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
