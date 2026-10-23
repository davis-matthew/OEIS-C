/*
 * A169737  a(1) = 100; for n>1, a(n) = a(n-1) + digitsum(a(n-1)).
 * https://oeis.org/A169737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 100; For n > 1, a(n) = a(n-1) + digit_sum(a(n-1)) (base
 *            10)
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;   /* dynamic array of computed terms (0‑based) */
static idx_t cache_len = 0;   /* number of elements currently stored */

/* Ensure that cache holds terms up to (but not including) index n */
static void ensure_capacity(idx_t n) {
    if (n <= cache_len) return;

    /* allocate new memory block for n elements */
#ifdef __cplusplus
    seq_t *new_cache = static_cast<seq_t*>(operator new(n * sizeof(seq_t)));
#else
    seq_t *new_cache = (seq_t*)malloc(n * sizeof(seq_t));
#endif
    if (!new_cache) abort();      /* out of memory */

    /* copy any already computed terms */
    for (idx_t i = 0; i < cache_len; ++i) {
#ifdef __cplusplus
        new (new_cache + i) seq_t(cache[i]);   /* copy‑construct */
#else
        new_cache[i] = cache[i];
#endif
    }

    /* free old storage and replace with the new block */
#ifdef __cplusplus
    operator delete[](cache);
#else
    free(cache);
#endif
    cache = new_cache;

    /* fill in the missing terms */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)100;
        } else {
            seq_t prev = cache[i-1];
            seq_t s    = digit_sum(prev);
            cache[i]   = prev + s;
        }
    }

    cache_len = n;
}

seq_t a(idx_t n) {
    /* The OEIS offset is 1; for n < 1 the behaviour is undefined. */
    if (n == 0) return (seq_t)0;   /* harmless fallback */

    ensure_capacity(n);
    return cache[n-1];
}
