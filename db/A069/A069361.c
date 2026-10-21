/*
 * A069361  Number of 3 X n binary arrays with a path of adjacent 1's from top
 *          row to bottom row.
 * https://oeis.org/A069361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 58*a(n-2) + 16*a(n-3) with initial values:
 *            a(1)=1, a(2)=17, a(3)=197
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache */
static idx_t cache_size = 0;     /* current allocated size (max index+1) */

/* ensure cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;
    idx_t new_size = (n < 1024) ? 1024 : n * 2 + 1; /* grow factor */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1); /* allocation failure */
    cache = new_cache;
    /* initialize newly allocated entries lazily */
    for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* out of range: safe value */
    ensure_cache(n);

    /* base cases */
    if (n == 1) {
        cache[1] = (seq_t)1;
        return cache[1];
    }
    if (n == 2) {
        cache[2] = (seq_t)17;
        return cache[2];
    }
    if (n == 3) {
        cache[3] = (seq_t)197;
        return cache[3];
    }

    /* compute missing terms, starting from next index after last computed */
    idx_t start = 1;
    while (start < cache_size && cache[start] != 0) start++;
    if (start < 4) start = 4; /* first unknown may be 4 */

    /* fill from 'start' up to n */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)15 * cache[i-1]
                 - (seq_t)58 * cache[i-2]
                 + (seq_t)16 * cache[i-3];
    }
    return cache[n];
}
