/*
 * A211117  Number of (n+1) X (n+1) -2..2 symmetric matrices with every 2 X 2
 *          subblock having sum zero and two, three or four distinct values.
 * https://oeis.org/A211117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 3*a(n-3) + 8*a(n-4) - 3*a(n-5) -
 *            2*a(n-6) (with a(1)=12, a(2)=30, a(3)=72, a(4)=174, a(5)=422,
 *            a(6)=1028)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.
       cache[0] holds a(1), cache[1] holds a(2), ...            */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t size     = 0;

    if (n == (idx_t)0)   /* undefined for n<1, return 0 as a sentinel */
        return (seq_t)0;

    /* Initialise the cache with the first six terms. */
    if (cache == NULL) {
        capacity = 64;                      /* initial capacity */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();                /* allocation failed */
        cache[0] = (seq_t)12;   /* a(1) */
        cache[1] = (seq_t)30;   /* a(2) */
        cache[2] = (seq_t)72;   /* a(3) */
        cache[3] = (seq_t)174;  /* a(4) */
        cache[4] = (seq_t)422;  /* a(5) */
        cache[5] = (seq_t)1028; /* a(6) */
        size = 6;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if ((size_t)n > size) {
        size_t need = (size_t)n;
        if (need > capacity) {
            size_t newcap = capacity;
            while (newcap < need)
                newcap <<= 1;            /* double until big enough */
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failed */
            cache = tmp;
            capacity = newcap;
        }

        /* Compute missing terms using the recurrence. */
        for (size_t m = size + 1; m <= need; ++m) {
            seq_t a1 = cache[m - 2]; /* a(m-1) */
            seq_t a2 = cache[m - 3]; /* a(m-2) */
            seq_t a3 = cache[m - 4]; /* a(m-3) */
            seq_t a4 = cache[m - 5]; /* a(m-4) */
            seq_t a5 = cache[m - 6]; /* a(m-5) */
            seq_t a6 = cache[m - 7]; /* a(m-6) */

            seq_t val = (seq_t)6 * a1
                      - (seq_t)11 * a2
                      + (seq_t)3  * a3
                      + (seq_t)8  * a4
                      - (seq_t)3  * a5
                      - (seq_t)2  * a6;
            cache[m - 1] = val;
        }
        size = need;
    }

    return cache[(size_t)n - 1];
}
