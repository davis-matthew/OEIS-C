/*
 * A027412  a(n) = 2*a(n-1) + (n-2)*a(n-2) with a(0) = 1, a(1) = 2.
 * https://oeis.org/A027412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + (n-2)*a(n-2); a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoized recurrence cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute the n-th term of A027412 */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    size_t req = (size_t)n;
    if (cache_cap <= req) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap <= req) new_cap = req + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base values (idempotent assignment) */
    cache[0] = (seq_t)1;
    if (req >= 1) cache[1] = (seq_t)2;

    /* Compute missing terms up to n */
    for (size_t i = 2; i <= req; ++i) {
        seq_t term = (seq_t)2 * cache[i-1] + (seq_t)(i-2) * cache[i-2];
        cache[i] = term;
    }

    return cache[req];
}
