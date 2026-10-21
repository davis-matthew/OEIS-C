/*
 * A099531  Expansion of (1+x)^3/((1+x)^3+x^4).
 * https://oeis.org/A099531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1; a(1)=a(2)=a(3)=0; recurrence: a(n) =
 *            -3*a(n-1)-3*a(n-2)-a(n-3)-a(n-4) for n>=4
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 44, i64 n <= 88, i128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for idx_t and seq_t types */

/* static cache storing values up to the highest n computed so far */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored (cache[0..cache_size-1]) */

/* Ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n) {
    if (cache_size > n) return;          /* already computed */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    /* resize the array */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – terminate the program (should never happen in tests) */
        abort();
    }
    cache = new_cache;
    /* compute newly required values */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1 || i == 2 || i == 3) {
            cache[i] = (seq_t)0;
        } else {
            /* recurrence uses already computed previous terms */
            cache[i] = - (seq_t)3 * cache[i-1]
                       - (seq_t)3 * cache[i-2]
                       -          cache[i-3]
                       -          cache[i-4];
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
