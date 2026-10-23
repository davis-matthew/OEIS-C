/*
 * A181358  Number of twiddle factors in the first stage of a Pease Radix 4
 *          Fast Fourier Transform.
 * https://oeis.org/A181358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) with a(0)=8, a(1)=44
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort; the caller will not recover. */
        abort();
    }
    cache = tmp;

    /* Compute new values. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)8;
        else if (i == 1)
            cache[1] = (seq_t)44;
        else {
            seq_t t1 = 5 * cache[i - 1];
            seq_t t2 = 4 * cache[i - 2];
            cache[i] = t1 - t2;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
