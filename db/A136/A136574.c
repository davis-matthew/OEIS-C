/*
 * A136574  Row sums of triangle A136573.
 * https://oeis.org/A136574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n! - 1 (a(0) = 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t last = 0;          /* index of last computed value   */
    static seq_t fact = 1;          /* factorial of 'last'   */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) = 1                */
        last   = 0;
        fact   = 1;                 /* 0! = 1                  */
    }

    /* grow the cache if required */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        free(cache);
        cache     = tmp;
        capacity  = new_cap;
    }

    /* return cached value if already computed */
    if (n <= last) return cache[n];

    /* compute successive terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        fact *= (seq_t)i;                           /* i!  */
        cache[i] = cache[i - 1] + fact - (seq_t)1;  /* recurrence */
    }
    last = n;
    return cache[n];
}
