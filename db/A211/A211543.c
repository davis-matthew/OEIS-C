/*
 * A211543  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w=3x+5y.
 * https://oeis.org/A211543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) - a(n-6) - a(n-8) +
 *            a(n-9) Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1,
 *            a(5)=1, a(6)=1, a(7)=2, a(8)=3
 * Range:     u8 n <= 63, u16 n <= 992, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated size of cache array */
    static size_t len   = 0; /* next index to be filled  (0..len-1 are valid) */

    /* Resize cache if necessary */
    if (cap < (size_t)(n) + 1) {
        size_t newcap = cap ? cap << 1U : 16U;
        if (newcap < (size_t)(n) + 1)
            newcap = (size_t)(n) + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Initialise the first nine terms on first call */
    if (len == 0) {
        /* Ensure space for the initial terms */
        if (cap < 9) {
            cap = 9;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[0] = 0; /* a(0) */
        cache[1] = 0; /* a(1) */
        cache[2] = 0; /* a(2) */
        cache[3] = 0; /* a(3) */
        cache[4] = 1; /* a(4) */
        cache[5] = 1; /* a(5) */
        cache[6] = 1; /* a(6) */
        cache[7] = 2; /* a(7) */
        cache[8] = 3; /* a(8) */
        len = 9;      /* next index to compute */
    }

    /* Compute any missing terms up to n */
    if (len <= (size_t)n) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            /* recurrence holds for i >= 9; for i < 9 values are already set */
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4]
                       + cache[i-5] - cache[i-6]
                       - cache[i-8] + cache[i-9];
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
