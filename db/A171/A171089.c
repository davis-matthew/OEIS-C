/*
 * A171089  a(n) = 2*(Lucas(n)^2 - (-1)^n).
 * https://oeis.org/A171089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(Lucas(n)^2 - (-1)^n) Sequence computed via recurrence:
 *            a(0) = 6, a(1) = 4, a(2) = 16 a(n) = 2*a(n-1) + 2*a(n-2) -
 *            a(n-3) (n ≥ 3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with first three values */
    if (cache_size == 0) {
        size_t cap = 3;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely */
        cache[0] = 6;
        cache[1] = 4;
        cache[2] = 16;
        cache_size = 3;
    }

    if (n < cache_size)
        return cache[n];

    /* Grow cache to hold values up to n */
    idx_t needed = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;          /* unlikely */
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)       val = 6;
        else if (i == 1)  val = 4;
        else if (i == 2)  val = 16;
        else              val = 2 * cache[i-1] + 2 * cache[i-2] - cache[i-3];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
