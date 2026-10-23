/*
 * A132592  X-values of solutions to the equation X*(X + 1) - 8*Y^2 = 0.
 * https://oeis.org/A132592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=8 and a(n)=34*a(n-1)-a(n-2)+16
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)8;
        cache_size = 2;
    }
    if (n < cache_size) return cache[n];
    for (idx_t i = cache_size; i <= n; ++i) {
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        seq_t next = (seq_t)34 * cache[i - 1] - cache[i - 2] + (seq_t)16;
        cache[i] = next;
    }
    cache_size = n + 1;
    return cache[n];
}
