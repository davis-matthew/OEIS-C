/*
 * A037116  Trajectory of 3 under map n->37n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3; a(n) = a(n-1)/2  if a(n-1) is even,
   otherwise a(n) = 37*a(n-1)+1 (for n>0) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (!cache) {
        cache_cap = 16;                                /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                  /* allocation failed */
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    while (cache_len <= (size_t)n) {
        if (cache_len >= cache_cap) {                  /* grow cache */
            size_t new_cap = cache_cap * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;                /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t cur = cache[cache_len - 1];
        seq_t next;
        if ((cur & (seq_t)1) == (seq_t)0) {
            next = cur / (seq_t)2;
        } else {
            next = cur * (seq_t)37 + (seq_t)1;
        }
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[(size_t)n];
}
