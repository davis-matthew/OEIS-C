/*
 * A061801  a(n) = (7*6^n - 2)/5.
 * https://oeis.org/A061801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*6^n - 2)/5, a(0) = 1; recurrence: a(n) = 6*a(n-1)+2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* external function computing the sequence */
seq_t a(idx_t n)
{
    /* static cache storage – initialized lazily */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* unlikely, safety */
        cache[0] = (seq_t)1;                         /* a(0) */
        cache_len = 1;
    }

    /* enlarge cache capacity if requested index n is beyond it */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* safety */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence a(k)=6*a(k-1)+2 */
    while (cache_len <= (size_t)n) {
        seq_t next = cache[cache_len - 1] * (seq_t)6 + (seq_t)2;
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[(size_t)n];
}
