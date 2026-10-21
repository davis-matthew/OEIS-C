/*
 * A214829  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 1, a(1) = a(2) = 7.
 * https://oeis.org/A214829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + a(n-3);
 * initial values: a(0) = 1, a(1) = a(2) = 7 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to handle repeated or non‑sequential calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* grow the cache if needed */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            sizeof(seq_t) * (n + 1));
        if (new_cache == NULL)
            return 0; /* allocation failure; caller should not happen */

        cache = new_cache;

        /* fill new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1 || i == 2)
                cache[i] = (seq_t)7;
            else
                cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
        cache_size = n + 1;
    }
    return cache[n];
}
