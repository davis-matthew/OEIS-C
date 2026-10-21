/*
 * A081588  Fourth row of the Pascal-(1,4,1) array A081579.
 * https://oeis.org/A081588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (125*n^3 - 150*n^2 + 115*n + 6)/6
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 591, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t n) {
    if (cache_size > n) return;
    idx_t new_cap = n + 1;
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[new_cap];
#else
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (cache == NULL) exit(1);
    } else {
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        delete [] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        cache = new_cache;
#endif
    }
    if (cache_size < 4) {
        cache[0] = 1;
        cache[1] = 16;
        cache[2] = 106;
        cache[3] = 396;
    }
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4) continue;
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];
        seq_t val = v1 * 4 - v2 * 6 + v3 * 4 - v4;
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n) {
    if (n < 4) {
        switch ((unsigned)n) {
            case 0: return 1;
            case 1: return 16;
            case 2: return 106;
            case 3: return 396;
            default: return 0; /* unreachable */
        }
    }
    ensure_capacity(n);
    return cache[n];
}
