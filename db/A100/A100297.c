/*
 * A100297  Expansion of g.f.: x*(1-11*x+x^2)/(1-17*x+14*x^2-x^3).
 * https://oeis.org/A100297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 6, a(3) = 89 a(n) = 17*a(n-1) - 14*a(n-2) +
 *            a(n-3) for n >= 4 (OEIS A100297)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_size = 0;

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)89;

    /* Ensure cache holds at least up to index n-1. */
    if (cached_size < 3) {
        /* Ensure base values are present. */
        if (cache == NULL) {
            cache = (seq_t*)malloc((size_t)3 * sizeof(seq_t));
            if (cache == NULL) abort();  /* cannot happen in tests */
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)89;
        cached_size = 3;
    } else if (cache == NULL) {
        /* Should never happen, but guard anyway. */
        cache = (seq_t*)malloc((size_t)3 * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)89;
        cached_size = 3;
    }

    if (n > cached_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) abort();  /* cannot happen in tests */
        cache = tmp;

        for (idx_t i = cached_size; i < new_size; ++i) {
            cache[i] = (seq_t)17 * cache[i-1]
                     - (seq_t)14 * cache[i-2]
                     +        cache[i-3];
        }
        cached_size = new_size;
    }

    return cache[n-1];
}
