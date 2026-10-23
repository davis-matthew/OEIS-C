/*
 * A008878  3x+1 sequence starting at 39.
 * https://oeis.org/A008878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 39, a(n+1) = (a(n) % 2 ? 3*a(n)+1 : a(n)/2) */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#ifdef __cplusplus
        /* construct the single element for C++ arbitrary–precision types */
        new(&cache[0]) seq_t();
#endif
        cache[0] = (seq_t)39;
        cache_len = 1;
    }

    /* grow cache to include n */
    while (cache_len <= (size_t)n) {
        seq_t cur = cache[cache_len - 1];
        seq_t next = (cur % 2 == 1) ? (cur * 3 + 1) : (cur / 2);

        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#ifdef __cplusplus
            /* construct newly allocated elements for C++ types */
            for (size_t i = cache_cap; i < new_cap; ++i) {
                new(&tmp[i]) seq_t();
            }
#endif
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[(size_t)n];
}
