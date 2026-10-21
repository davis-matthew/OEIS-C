/*
 * A233157  Number of 4 X n 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally or antidiagonally.
 * https://oeis.org/A233157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 15*a(n-2) + 6*a(n-3) for n > 7 with initial
 *            values a(1)=81, a(2)=384, a(3)=2592, a(4)=17496, a(5)=121176,
 *            a(6)=842616, a(7)=5871528 (OEIS entries A233157) The sequence is
 *            computed by memoised recurrence, storing results in a static
 *            dynamic array
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                    /* undefined for n<1 */

    /* Static memoisation table */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size */
    static size_t len = 0;          /* number of entries computed (max index) */

    /* Ensure capacity for index n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 8;
        while (newcap < (size_t)n + 1) newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        /* initialise new elements to zero */
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (size_t idx = len; idx < (size_t)n; ++idx) {
        size_t i = idx + 1;                 /* table index starts at 1 */
        if (i == 1) cache[i] = (seq_t)81;
        else if (i == 2) cache[i] = (seq_t)384;
        else if (i == 3) cache[i] = (seq_t)2592;
        else if (i == 4) cache[i] = (seq_t)17496;
        else if (i == 5) cache[i] = (seq_t)121176;
        else if (i == 6) cache[i] = (seq_t)842616;
        else if (i == 7) cache[i] = (seq_t)5871528;
        else {
            /* Recurrence: a(n) = 9*a(n-1) - 15*a(n-2) + 6*a(n-3) */
            seq_t t1 = mul_small(cache[i-1], 9);
            seq_t t2 = mul_small(cache[i-3], 6);
            seq_t t3 = mul_small(cache[i-2], 15);
            seq_t val = t1;
            val += t2;
            val -= t3;
            cache[i] = val;
        }
    }
    len = (size_t)n;   /* now we have computed up to index n */

    return cache[n];
}
