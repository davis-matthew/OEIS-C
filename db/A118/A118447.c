/*
 * A118447  Number of rooted n-edge one-vertex maps on the Klein bottle
 *          (dually: one-face maps).
 * https://oeis.org/A118447
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   -(n-2)*(n-1)^2*a(n) + 2*n*(4*n-5)*(n-2)*a(n-1) -
 *            8*n*(n-1)*(2*n-3)*a(n-2) = 0 with a(2)=4, a(3)=42
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Grow the cache to accommodate index n */
static void grow_cache(idx_t n) {
    if ((size_t)n + 1 <= cache_size) return;          /* already big enough */

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new (std::nothrow) seq_t[new_size];
    if (!new_cache) exit(1);                          /* out of memory */
#else
    new_cache = (seq_t*)malloc(sizeof(seq_t)*new_size);
    if (!new_cache) exit(1);                          /* out of memory */
#endif

    /* copy existing values */
    size_t old = cache_size;
    for (size_t i = 0; i < old; ++i) new_cache[i] = cache[i];

    /* free old storage */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_size = new_size;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* undefined below offset */

    size_t old_size = cache_size; /* indices already filled */
    grow_cache(n);

    /* Fill missing values */
    for (idx_t k = old_size; k <= n; ++k) {
        if (k == 0 || k == 1) { cache[k] = 0; continue; }
        if (k == 2) { cache[2] = 4; continue; }
        if (k == 3) { cache[3] = 42; continue; }

        /* recurrence:
           a(k) = [2*k*(4*k-5)*(k-2)*a(k-1) - 8*k*(k-1)*(2*k-3)*a(k-2)] / ((k-2)*(k-1)^2)
        */
        seq_t t1 = cache[k-1];
        t1 *= (seq_t)k;
        t1 *= 2;
        t1 *= (seq_t)(4*k-5);
        t1 *= (k-2);

        seq_t t2 = cache[k-2];
        t2 *= (seq_t)k;
        t2 *= 8;
        t2 *= (k-1);
        t2 *= (seq_t)(2*k-3);

        seq_t num = t1 - t2;
        seq_t denom = (seq_t)(k-2) * (k-1) * (k-1);
        cache[k] = num / denom;
    }

    return cache[n];
}
