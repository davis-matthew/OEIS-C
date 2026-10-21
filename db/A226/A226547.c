/*
 * A226547  Number of squares in all tilings of a 4 X n rectangle using
 *          integer-sided square tiles.
 * https://oeis.org/A226547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) -12*a(n-3) -11*a(n-4) + 2*a(n-5)
 *            +10*a(n-6) +6*a(n-7) - a(n-8) -2*a(n-9) - a(n-10) with initial
 *            values a(0)=0, a(1)=4, a(2)=25, a(3)=98, a(4)=386, a(5)=1402,
 *            a(6)=4938, a(7)=16936, a(8)=57020, a(9)=189172, a(10)=620397
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;

    /* Allocate and initialize cache on first call */
    if (!cache) {
        /* Allocate space for indices 0..10 (11 values) */
        cache = (seq_t *)calloc((size_t)11, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure → undefined */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)25;
        cache[3] = (seq_t)98;
        cache[4] = (seq_t)386;
        cache[5] = (seq_t)1402;
        cache[6] = (seq_t)4938;
        cache[7] = (seq_t)16936;
        cache[8] = (seq_t)57020;
        cache[9] = (seq_t)189172;
        cache[10] = (seq_t)620397;
        cached_up_to = 10;
    }

    /* Grow cache if requested index is beyond current size */
    if (n > cached_up_to) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* reallocation failure → undefined */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t pos = (seq_t)0;
            seq_t neg = (seq_t)0;

            /* Positive contributions */
            pos += (seq_t)4   * cache[i - 1];
            pos += (seq_t)2   * cache[i - 2];
            pos += (seq_t)2   * cache[i - 5];
            pos += (seq_t)10  * cache[i - 6];
            pos += (seq_t)6   * cache[i - 7];

            /* Negative contributions */
            neg += (seq_t)12  * cache[i - 3];
            neg += (seq_t)11  * cache[i - 4];
            neg +=           cache[i - 8];
            neg += (seq_t)2   * cache[i - 9];
            neg +=           cache[i -10];

            cache[i] = pos - neg; /* pos >= neg for all valid indices */
        }
        cached_up_to = n;
    }
    return cache[n];
}
