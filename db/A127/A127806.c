/*
 * A127806  Row sums of number triangle A127805.
 * https://oeis.org/A127806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, * a(2k) = 10^(2k), * a(2k+1) = 10^(2k+1) + a(k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of allocated elements   */
static seq_t pow_last;         /* 10^pow_index                   */
static idx_t pow_index = 0;    /* index that pow_last represents */

static void ensure_cache(idx_t n) {
    if (n + 1 <= cache_size)
        return;
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    for (idx_t i = cache_size; i <= n; ++i)
        tmp[i] = 0;            /* newly added slots */
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    /* one‑time initialization */
    if (cache_size == 0) {
        ensure_cache(0);
        cache[0] = 1;
        pow_last = 1;
        pow_index = 0;
    }
    ensure_cache(n);
    if (n == 0)
        return cache[0];
    if (cache[n] != 0)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = pow_index + 1; i <= n; ++i) {
        pow_last = pow_last * 10;          /* now pow_last = 10^i */
        if ((i & 1) == 0)
            cache[i] = pow_last;          /* even index */
        else
            cache[i] = pow_last + cache[(i - 1) / 2];
    }
    pow_index = n;
    return cache[n];
}
