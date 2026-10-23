/*
 * A025971  Expansion of 1/((1-2*x)*(1-4*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A025971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 154*a(n-2) + 456*a(n-3) - 448*a(n-4) with
 *            a(0)=1, a(1)=21, a(2)=287, a(3)=3249
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;  /* dynamically grow cache on demand */
    static idx_t max_n = 0;      /* number of entries allocated (max_n-1 is last valid index) */

    /* If requested term is already cached, return it immediately */
    if (n < max_n)
        return cache[n];

    /* Increase cache size to include index n (inclusive) */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort with diagnostic (could also set errno) */
        abort();
    }
    cache = new_cache;

    /* Compute all new entries from max_n up to n */
    for (idx_t i = max_n; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)21;
        else if (i == 2)
            cache[i] = (seq_t)287;
        else if (i == 3)
            cache[i] = (seq_t)3249;
        else
            cache[i] = (seq_t)21  * cache[i-1]
                     - (seq_t)154 * cache[i-2]
                     + (seq_t)456 * cache[i-3]
                     - (seq_t)448 * cache[i-4];
    }
    max_n = new_size; /* now cache holds indices [0..n] */

    return cache[n];
}
