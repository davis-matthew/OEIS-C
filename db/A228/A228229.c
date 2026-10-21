/*
 * A228229  Recurrence a(n) = n*(n + 1)*a(n-1) + 1 with a(0) = 1.
 * https://oeis.org/A228229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*a(n-1) + 1 with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cache_size = 0;     /* current allocated size */
    static idx_t last_computed = 0;  /* highest index already computed */
    static int first_call = 1;       /* flag for initialisation */

    /* Allocate or grow the cache to hold indices up to n */
    if (first_call || n + 1 > cache_size) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;
        cache_size = new_cap;
        if (first_call) {
            cache[0] = (seq_t)1;    /* a(0) */
            last_computed = 0;
            first_call = 0;
        }
    }

    /* If the requested value is already cached, return it */
    if (n <= last_computed) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = i*(i+1)*a(i-1) + 1 */
        seq_t i_seq = (seq_t)i;
        cache[i] = i_seq * (i_seq + (seq_t)1) * cache[i - 1] + (seq_t)1;
    }
    last_computed = n;

    return cache[n];
}
