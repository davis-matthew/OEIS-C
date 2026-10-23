/*
 * A014829  a(1)=1, a(n) = 6*a(n-1) + n.
 * https://oeis.org/A014829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 6*a(n-1) + n for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif


static seq_t *seq_cache = NULL;
static idx_t cache_n = 0;

/* Storage helpers usable in C and C++ */
static void *alloc_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return malloc(n * sizeof(seq_t));
#endif
}

static void free_array(void *p) {
#ifdef __cplusplus
    delete[] static_cast<seq_t*>(p);
#else
    free(p);
#endif
}

static seq_t *realloc_array(seq_t *old, idx_t old_count, idx_t new_count) {
#ifdef __cplusplus
    seq_t* newp = new seq_t[new_count];
    for (idx_t i = 1; i <= old_count; ++i) newp[i] = old[i];
    delete[] old;
    return newp;
#else
    return (seq_t*)realloc(old, new_count * sizeof(seq_t));
#endif
}

/* Compute A014829: a(1)=1, a(n)=6*a(n-1)+n */
seq_t a(idx_t n) {
    if (n < 1) return 0;            /* behaviour for n < 1 is arbitrary */

    /* first allocation */
    if (seq_cache == NULL) {
        seq_cache = (seq_t*)alloc_array((size_t)(n + 1));
        if (!seq_cache) return 0;
        seq_cache[1] = (seq_t)1;
        cache_n = 1;
        for (idx_t i = 2; i <= n; ++i)
            seq_cache[i] = (seq_t)6 * seq_cache[i - 1] + i;
        cache_n = n;
        return seq_cache[n];
    }

    /* extend cache if needed */
    if (n > cache_n) {
        seq_cache = (seq_t*)realloc_array(seq_cache, cache_n, n + 1);
        if (!seq_cache) return 0;
        for (idx_t i = cache_n + 1; i <= n; ++i)
            seq_cache[i] = (seq_t)6 * seq_cache[i - 1] + i;
        cache_n = n;
    }

    return seq_cache[n];
}
