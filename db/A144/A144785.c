/*
 * A144785  Variant of Sylvester's sequence: a(n+1) = a(n)^2 - a(n) + 1, with
 *          a(1) = 12.
 * https://oeis.org/A144785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=12, a(n+1)=a(n)^2 - a(n) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;     /* stored values a(1) .. a(cache_len) */
static size_t cache_cap = 0;    /* allocated capacity (in elements) */
static idx_t cache_len = 0;     /* number of elements currently stored   */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Out of bounds, return 0 */

    /* Ensure we have space for n elements */
    if (cache == NULL) {
        /* Initial allocation large enough for n but at least 1 */
        size_t init_cap = n > 1 ? n : 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        cache_cap = init_cap;
        cache_len = 0;
    } else if (cache_cap < (size_t)n) {
        /* Grow until capacity is at least n (doubling strategy) */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Initialize first term if not yet set */
    if (cache_len == 0) {
        cache[0] = (seq_t)12;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = prev * prev - prev + (seq_t)1;
        cache[i] = next;
    }
    cache_len = n;

    return cache[n - 1];
}
