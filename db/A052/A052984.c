/*
 * A052984  a(n) = 5*a(n-1) - 2*a(n-2) for n>1, with a(0) = 1, a(1) = 3.
 * https://oeis.org/A052984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2), with a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Seed the cache with the first two terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of terms currently cached */
    static idx_t capacity = 0;    /* allocated capacity of the cache */

    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!cache) return (seq_t)0;   /* allocation failure – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cached = 2;
        return n == 0 ? cache[0] : cache[1];
    }

    /* Grow the cache if needed */
    if ((idx_t)n + 1 > capacity) {
        idx_t newcap = capacity * 2;
        if (newcap < (idx_t)n + 1)
            newcap = (idx_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute values up to n */
    while (cached <= n) {
        seq_t val = (seq_t)5 * cache[cached - 1];
        val -= (seq_t)2 * cache[cached - 2];
        cache[cached] = val;
        ++cached;
    }

    return cache[n];
}
