/*
 * A219161  Recurrence equation a(n+1) = a(n)^3 - 3*a(n) with a(0) = 5.
 * https://oeis.org/A219161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; a(n+1) = a(n)^3 - 3*a(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n >= cache_len) {
        /* Grow cache to hold terms up to n */
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed – return 0 */

        /* Compute new terms */
        for (size_t i = cache_len; i < newlen; ++i) {
            if (i == 0) {
                tmp[0] = (seq_t)5;
            } else {
                seq_t prev = tmp[i - 1];
                tmp[i] = prev * prev * prev - (seq_t)3 * prev;
            }
        }

        cache     = tmp;
        cache_len = newlen;
    }

    return cache[(size_t)n];
}
