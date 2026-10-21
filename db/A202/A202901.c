/*
 * A202901  Number of n X 3 0..1 arrays with every one equal to some NW, E or
 *          S neighbor.
 * https://oeis.org/A202901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 17*a(n-2) + 32*a(n-3) - 70*a(n-4) + 72*a(n-5)
 *            - 24*a(n-6) with initial values: a(1) = 1, a(2) = 14, a(3) = 93,
 *            a(4) = 494, a(5) = 2801, a(6) = 16062
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The type seq_t and idx_t are supplied by the build environment.  */
seq_t a(idx_t n)
{
    /* Static cache to store already computed terms.
       Indexes in the array correspond to the sequence index (1‑based). */
    static seq_t *cache   = NULL; /* 1‑based array, index 0 unused */
    static idx_t  length  = 0;    /* number of terms cached (max index stored) */

    /* Ensure that the cache can hold the requested term.  */
    if (n > length) {
        /* Grow the cache to at least n + 1 elements (index 0 unused).  */
        idx_t new_size = n + 1;
        if (new_size < 6 + 1) new_size = 6 + 1;           /* at least 6 terms */
        if (new_size < length * 2) new_size = length * 2; /* exponential growth */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        cache = tmp;
        /* If the cache was enlarged, initialize the new entries to zero. */
        for (idx_t i = length + 1; i < new_size; ++i) cache[i] = 0;
        length = new_size - 1;                    /* new length is new_size-1 */
    }

    /* Fill in missing terms up to n.  */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 6 && cache[i] == 0) {                 /* initial terms */
            switch (i) {
                case 1: cache[1] = (seq_t)1;    break;
                case 2: cache[2] = (seq_t)14;   break;
                case 3: cache[3] = (seq_t)93;   break;
                case 4: cache[4] = (seq_t)494;  break;
                case 5: cache[5] = (seq_t)2801; break;
                case 6: cache[6] = (seq_t)16062;break;
            }
        } else if (i > 6 && cache[i] == 0) {           /* recurrence */
            seq_t term1 = (seq_t)8      * cache[i-1];
            seq_t term2 = (seq_t)32     * cache[i-3];
            seq_t term3 = (seq_t)72     * cache[i-5];
            seq_t positive = term1 + term2 + term3;

            seq_t neg1 = (seq_t)17     * cache[i-2];
            seq_t neg2 = (seq_t)70     * cache[i-4];
            seq_t neg3 = (seq_t)24     * cache[i-6];
            seq_t negative = neg1 + neg2 + neg3;

            cache[i] = positive - negative;
        }
    }

    return cache[n];
}
