/*
 * A062187  a(n+1) = a(n) - a(floor(n/2)), with a(0)=0, a(1)=1.
 * https://oeis.org/A062187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A062187, a(n+1) = a(n) - a(floor(n/2)), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 37, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms. */
static seq_t *cache      = NULL;
static idx_t  capacity   = 0;
static idx_t  used_index = 0;   /* next index to compute (size of cache) */

/* Ensure the cache holds at least up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < used_index)
        return;   /* already computed */

    /* Grow capacity if necessary. */
    if (capacity == 0)
        capacity = 4;
    while (capacity <= n)
        capacity <<= 1;          /* double until large enough */

    /* Reallocate memory.  The cast is required for C++ builds where
       seq_t may be a non-POD type. */
    seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    if (tmp == NULL) {
        /* Out of memory – leave cache unchanged; subsequent calls will
           return 0. */
        return;
    }
    cache = tmp;

    /* Compute any missing terms. */
    for (idx_t i = used_index; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)0;
        else if (i == 1)
            cache[1] = (seq_t)1;
        else {
            idx_t half = (i - 1) >> 1;   /* floor((i-1)/2) */
            cache[i] = cache[i - 1] - cache[half];
        }
    }
    used_index = n + 1;   /* new size of cache */
}

/* Public function. */
extern seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < used_index)
        return cache[n];
    return (seq_t)0;   /* fallback on memory error */
}
