/*
 * A145303  a(n) = ((8 + sqrt(8))^n + (8 - sqrt(8))^n)/2.
 * https://oeis.org/A145303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(n)=16*a(n-1)-56*a(n-2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else if (i == 1)
                new_cache[i] = (seq_t)8;
            else
                new_cache[i] = 16 * new_cache[i - 1] - 56 * new_cache[i - 2];
        }

        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
