/*
 * A206144  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2); a(1)=60, a(2)=246, a(3)=1122,
 *            a(4)=5118
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial or out-of-bounds requests */
    if (n == 0) return (seq_t)0;
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)60;
            case 2: return (seq_t)246;
            case 3: return (seq_t)1122;
            case 4: return (seq_t)5118;
        }
    }

    /* Memoization cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t last = 0;   /* highest index computed so far */
    static size_t cap   = 0;   /* allocated capacity of cache */

    /* Allocate or enlarge cache on first request or when needed */
    if (cache == NULL) {
        cap = (n > 4 ? n : 5);
        cache = (seq_t*)malloc(sizeof(seq_t) * (cap + 1));
        if (!cache) return (seq_t)0; /* allocation failure */
        /* Store initial values */
        cache[1] = (seq_t)60;
        cache[2] = (seq_t)246;
        cache[3] = (seq_t)1122;
        cache[4] = (seq_t)5118;
        last = 4;
    } else if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) return (seq_t)0; /* reallocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t term = 5 * cache[i - 1] - 2 * cache[i - 2];
        cache[i] = term;
    }
    last = (size_t)n;
    return cache[n];
}
