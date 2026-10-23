/*
 * A165150  a(n) = (3*10^n - 6^n)/2.
 * https://oeis.org/A165150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=12, a(n)=16*a(n-1)-60*a(n-2) for n>1 */
/* This computes a(n) = (3*10^n - 6^n)/2 exactly for all supported seq_t types. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* current allocated length */

    /* Initial allocation for n = 0 and 1 */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;     /* allocation failure, return 0 as sentinel */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
    }

    /* Resize cache if needed */
    if ((idx_t)cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure */
        cache = tmp;
        /* Compute missing terms using the recurrence */
        for (idx_t i = cap; i <= n; ++i) {
            cache[i] = (seq_t)(16 * cache[i - 1] - 60 * cache[i - 2]);
        }
        cap = new_cap;
    }

    return cache[n];
}
