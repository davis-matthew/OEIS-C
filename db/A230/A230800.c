/*
 * A230800  Number of n X 2 0..3 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 4 and at least one element with value (x(i,j)-1) mod 4, and
 *          upper left element zero.
 * https://oeis.org/A230800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 5*a(n-3) for n ≥ 4, with a(1) = 0,
 *            a(2) = 2, a(3) = 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_cache(idx_t n, seq_t **cache, size_t *cap) {
    if (*cache == NULL) {
        size_t init = (n + 1 > 4) ? n + 1 : 4;
        *cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!*cache) exit(1);
        *cap   = init;
    } else if (*cap <= n) {
        size_t newcap = *cap * 2;
        if (newcap <= n) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        *cache = tmp;
        *cap   = newcap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    static seq_t *cache = NULL;     /* 1‑indexed array of values */
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;    /* maximum index that has been computed */

    /* ensure the cache has enough space */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base values once */
    if (cache_len == 0) {
        cache[1] = (seq_t)0;
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[2] = (seq_t)2;
        cache_len = 2;
    }
    if (cache_len < 3) {
        cache[3] = (seq_t)2;
        cache_len = 3;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + 6 * cache[i - 2] - 5 * cache[i - 3];
    }
    cache_len = n;

    return cache[n];
}
