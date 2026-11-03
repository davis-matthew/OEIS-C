/*
 * A036231  a(n+1) = a(n) + sum of digits of a(n) starting with 121.
 * https://oeis.org/A036231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 121, a(n+1) = a(n) + digit_sum(a(n))
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A036231: 121, 125, 133, 140, 145, 155, 166, 179, 196, 212, 217, 227, ... */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;        /* previously computed terms */
static idx_t cache_size = 0;       /* number of terms stored   */
static idx_t cache_cap  = 0;       /* allocated capacity       */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;

    idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* out of bounds – not used */

    idx_t pos = n - 1;             /* zero‑based cache index */

    if (pos >= cache_size)
    {
        /* ensure storage */
        ensure_capacity(pos + 1);

        /* initialise first term if cache empty */
        if (cache_size == 0)
        {
            cache[0] = (seq_t)121; /* a(1) */
            cache_size = 1;
        }

        /* compute missing terms up to 'pos' */
        for (idx_t i = cache_size; i <= pos; ++i)
        {
            seq_t prev  = cache[i - 1];
            seq_t sumd  = digit_sum(prev);
            cache[i] = prev + sumd;
        }

        cache_size = pos + 1;
    }

    return cache[pos];
}
