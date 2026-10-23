/*
 * A123851  A cubic recurrence: a(0) = 1, a(n) = n*a(n-1)^3 for n >= 1.
 * https://oeis.org/A123851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = n * a(n-1)^3 (OEIS A123851)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Compute the n-th term of sequence A123851. */
seq_t a(idx_t n) {
    /* Static cache for memoised values.  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements in cache */

    /* Initialise cache for a(0). */
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[1];
#else
        cache = (seq_t *)malloc(sizeof(seq_t));
#endif
        if (cache == NULL) return 0;  /* out of memory */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Return cached value if available. */
    if (n < (idx_t)cache_len) {
        return cache[(size_t)n];
    }

    /* Grow cache to accommodate n. */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_len];
    /* Copy already computed values. */
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }
    delete[] cache;
#else
    new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL) return 0; /* out of memory */
    cache = new_cache; /* keep pointer for assignment */
#endif
    cache = new_cache;
    /* Compute new terms from the last computed index. */
    size_t last_index = cache_len - 1;
    for (idx_t i = (idx_t)last_index + 1; i <= n; ++i) {
        seq_t prev = cache[(size_t)i - 1];
        seq_t cube = prev * prev * prev;   /* prev^3 fits in seq_t */
        cache[(size_t)i] = ((seq_t)i) * cube;
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
