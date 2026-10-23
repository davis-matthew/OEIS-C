/*
 * A186161  1/4 the number of n X 2 0..3 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A186161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 11*a(n-2) + 9*a(n-3) - 9*a(n-4) - 27*a(n-5);
 *            Initial values: a(1)=1, a(2)=7, a(3)=48, a(4)=321, a(5)=2175
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 5;           /* last index computed */

    if (n < 1) return 0;               /* undefined for n<1 */

    /* allocate and init cache on first call */
    if (!cache) {
        cap = 32;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        /* 0‑th element unused */
        cache[0] = 0;
        /* initial five terms */
        cache[1] = 1;
        cache[2] = 7;
        cache[3] = 48;
        cache[4] = 321;
        cache[5] = 2175;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        while (newcap <= (size_t)n) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = 5 * cache[i-1] + 11 * cache[i-2] + 9 * cache[i-3]
                     - 9 * cache[i-4] - 27 * cache[i-5];
        }
        last = n;
    }

    return cache[n];
}
