/*
 * A080787  a(1)=a(2)=1; a(n) = a(n-1) + last decimal digit of a(n-2).
 * https://oeis.org/A080787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=a(2)=1; for n>=3 a(n)=a(n-1)+last decimal digit of a(n-2)
 * Range:     u8 n <= 55, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Advantage: simple O(n) calculation with caching for arbitrary call order. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    /* Static cache to avoid recomputation on repeated calls */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;     /* number of allocated slots (index 0 unused) */

    if (n + 1 > capacity) {
        /* Grow cache to accommodate n */
        idx_t new_capacity = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!new_cache) exit(1);  /* stop if out of memory */
        cache = new_cache;

        /* Initialize base terms if cache was empty */
        if (capacity == 0) {
            cache[1] = (seq_t)1;
            if (new_capacity > 2)
                cache[2] = (seq_t)1;
            capacity = 3;  /* indices 1 and 2 are known; compute from 3 upwards */
        }

        /* Compute new terms up to n */
        for (idx_t i = capacity; i <= n; ++i) {
            seq_t last_digit = cache[i - 2] % (seq_t)10;
            cache[i] = cache[i - 1] + last_digit;
        }
        capacity = n + 1;
    }

    return cache[n];
}
