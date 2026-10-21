/*
 * A093146  Third binomial transform of Pell(3n-1).
 * https://oeis.org/A093146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=5, a(n)=20*a(n-1)-50*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache will hold computed terms up to the largest n seen so far. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;  /* number of slots currently allocated */

    /* Ensure capacity for index n. */
    if (n >= cap) {
        /* Allocate or grow the cache. */
        idx_t new_cap = (n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; abort gracefully. */
            abort();
        }
        /* Initialize newly allocated entries. */
        for (idx_t i = cap; i < new_cap; ++i) {
            if (i == 0) {
                new_cache[i] = (seq_t)1;
            } else if (i == 1) {
                new_cache[i] = (seq_t)5;
            } else {
                /* Recurrence: a(i) = 20*a(i-1) - 50*a(i-2) */
                seq_t term1 = (seq_t)20 * new_cache[i - 1];
                seq_t term2 = (seq_t)50 * new_cache[i - 2];
                new_cache[i] = term1 - term2;
            }
        }
        cache = new_cache;
        cap   = new_cap;
    }
    return cache[n];
}
