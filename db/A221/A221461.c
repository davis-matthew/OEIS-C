/*
 * A221461  Number of 0..6 arrays of length n with each element unequal to at
 *          least one neighbor, starting with 0.
 * https://oeis.org/A221461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2), initial values: a(1) = 0, a(2) = 6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* initialise cache for n=1 and n=2 */
    if (cap == 0) {
        cap = 2;
        cache = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
    }

    /* return cached value if already computed */
    if (n <= cap)
        return cache[n];

    /* extend cache to at least n */
    idx_t newcap = cap;
    while (newcap < n)
        newcap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, (newcap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute values from (cap+1) up to n */
    for (idx_t i = cap + 1; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 1] + (seq_t)6 * cache[i - 2];
    }
    cap = n;
    return cache[n];
}
