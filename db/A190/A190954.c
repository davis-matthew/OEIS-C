/*
 * A190954  a(n) = 10*a(n-1) + 4*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 4*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_size = 0;   /* number of terms currently stored (next free index) */
static size_t cache_cap  = 0;   /* capacity of the cache array */

/* Computes the n-th term of the sequence A190954 */
seq_t a(idx_t n)
{
    /* Ensure base cases are stored */
    if (cache_size == 0) {
        size_t init_cap = 2;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        cache[i] = (seq_t)10 * cache[i-1] + (seq_t)4 * cache[i-2];
    }
    if ((size_t)n + 1 > cache_size)
        cache_size = (size_t)n + 1;

    return cache[n];
}
