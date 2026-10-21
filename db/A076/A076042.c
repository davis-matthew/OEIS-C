/*
 * A076042  a(0) = 0; thereafter a(n) = a(n-1) + n^2 if a(n-1) < n^2,
 *          otherwise a(n) = a(n-1) - n^2.
 * https://oeis.org/A076042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0; thereafter * a(n) = a(n-1)+n^2 if a(n-1) < n^2 * a(n)
 *            = a(n-1)-n^2 otherwise
 * Range:     u8 n <= 11, u16 n <= 182, u32 n <= 9820, u64 n <= 9820,
 *            u128 n <= 9820, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

typedef struct {
    seq_t *data;
    size_t len;   /* number of elements stored, indices 0..len-1 are filled */
    size_t cap;   /* allocated capacity */
} cache_t;

static cache_t cache = {NULL, 0, 0};

static void cache_grow(void) {
    size_t newcap = (cache.cap == 0) ? 64 : cache.cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache.data, newcap * sizeof(seq_t));
    if (!tmp) exit(1);            /* fatal error: out of memory */
    cache.data = tmp;
    cache.cap = newcap;
}

seq_t a(idx_t n) {
    /* ensure the cache holds the element at index n */
    while (cache.len <= n) {
        if (cache.len == cache.cap) cache_grow();

        if (cache.len == 0) {            /* initialize a(0) */
            cache.data[0] = 0;
            cache.len = 1;
            continue;
        }

        idx_t i = cache.len;                /* compute a(i) */
        seq_t prev = cache.data[i - 1];
        seq_t sq = (seq_t)i * (seq_t)i;      /* i^2 */

        seq_t val = (prev < sq) ? (prev + sq) : (prev - sq);
        cache.data[i] = val;
        cache.len++;
    }
    return cache.data[n];
}
