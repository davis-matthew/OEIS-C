/*
 * A158545  a(n) = Hermite(n,14).
 * https://oeis.org/A158545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 28*a(n-1) - 2*(n-1)*a(n-2); a(0)=1, a(1)=28
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cache for computed values */
static idx_t cached_size = 0;     /* number of entries currently cached */

seq_t a(idx_t n) {
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;

    /* allocate or grow cache if needed */
    if (cache == NULL) {
        cached_size = 2;
        cache = (seq_t*)malloc(cached_size * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
    }
    if (n >= cached_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        /* compute new entries */
        for (idx_t i = cached_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)28;
            } else {
                seq_t term1 = 28 * cache[i - 1];
                seq_t term2 = 2 * ((seq_t)(i - 1)) * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cached_size = new_size;
    }
    return cache[n];
}
