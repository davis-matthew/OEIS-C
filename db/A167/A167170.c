/*
 * A167170  a(6) = 14, for n >= 7, a(n) = a(n-1) + gcd(n, a(n-1)).
 * https://oeis.org/A167170
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(6) = 14; for n >= 7,
   a(n) = a(n-1) + gcd(n, a(n-1))     (OEIS A167170) */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 6 – the caller must not use those values. */
    if (n < 6) return (seq_t)0;

    static seq_t *cache = NULL;      /* Cached values a(6), a(7), … */
    static size_t   cache_size = 0;   /* Number of cached entries */
    static idx_t    last_n    = 5;     /* Highest n for which value is cached */

    size_t target_index = (size_t)(n - 6);   /* Index into cache for a(n) */

    if (n > last_n) {
        size_t new_size = target_index + 1;   /* Size needed to store a(n) */

        if (!cache) {                         /* First allocation */
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
            if (!cache) abort();

            /* a(6) = 14 */
            cache[0] = (seq_t)14;
            for (size_t i = 1; i < new_size; ++i) {
                idx_t cur  = 6 + (idx_t)i;
                seq_t prev = cache[i - 1];
                seq_t g    = gcd((seq_t)cur, prev);
                cache[i]   = prev + g;
            }
            cache_size = new_size;
            last_n     = n;
        } else {                                /* Grow existing cache */
            seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;

            for (size_t i = cache_size; i < new_size; ++i) {
                idx_t cur  = 6 + (idx_t)i;
                seq_t prev = cache[i - 1];
                seq_t g    = gcd((seq_t)cur, prev);
                cache[i]   = prev + g;
            }
            cache_size = new_size;
            last_n     = n;
        }
    }

    return cache[target_index];
}
