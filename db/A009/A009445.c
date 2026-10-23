/*
 * A009445  a(n) = (2*n+1)!.
 * https://oeis.org/A009445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2*n+1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;  a(n) = 2*n*(2*n+1)*a(n-1) for n ≥ 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already-computed values */
    static seq_t *cache   = NULL;
    static idx_t cached_len = 0;

    /* Ensure we have space for all indices up to n */
    if (n >= cached_len) {
        idx_t new_len = n + 1;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
            /* first term */
            cache[0] = (seq_t)1;
            cached_len = 1;
        } else if (new_len > cached_len) {
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached_len; i <= n; ++i) {
            seq_t term = cache[i-1];
            term = term * (seq_t)(2*i);          /* multiply by 2*i   */
            term = term * (seq_t)(2*i + 1);      /* multiply by 2*i+1 */
            cache[i] = term;
        }
        cached_len = new_len;
    }

    return cache[n];
}
