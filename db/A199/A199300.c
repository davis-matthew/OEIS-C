/*
 * A199300  a(n) = (2*n + 1)*7^n.
 * https://oeis.org/A199300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 21, a(2) = 245, a(3) = 2401, ... */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *pow7_cache = NULL;      /* 7^i stored for i = 0 .. cache_len-1 */
static size_t   cache_len  = 0;       /* number of elements currently valid */
static size_t   cache_cap  = 0;       /* allocated capacity */

static void
ensure_cache_capacity(size_t needed)
{
    if (cache_cap > needed)
        return;
    /* Grow: double until we have enough space */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc((void *)pow7_cache,
                                  new_cap * sizeof(seq_t));
    /* If realloc failed we keep the old pointer; in this simple
       implementation we simply abort (caller is expected to have enough
       memory). */
    if (!tmp)
        exit(EXIT_FAILURE);
    pow7_cache = tmp;
    cache_cap  = new_cap;
}

/* Return (2*n + 1) * 7^n */
seq_t
a(idx_t n)
{
    /* Build the cache up to the requested n if necessary */
    if (cache_len == 0) {
        /* initial cache: 7^0 = 1 */
        ensure_cache_capacity(0);
        pow7_cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if ((size_t)n >= cache_len) {
        ensure_cache_capacity((size_t)n);
        seq_t seven = (seq_t)7;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            pow7_cache[i] = pow7_cache[i-1] * seven;
        }
        cache_len = (size_t)n + 1;
    }

    seq_t two_n   = (seq_t)2 * (seq_t)n;
    seq_t factor  = two_n + (seq_t)1;
    return factor * pow7_cache[(size_t)n];
}
