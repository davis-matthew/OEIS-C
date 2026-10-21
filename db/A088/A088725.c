/*
 * A088725  Numbers having no divisors d>1 such that also d+1 is a divisor.
 * https://oeis.org/A088725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088725: numbers with no divisor d>1 such that d+1 is also a divisor.
   Method: linearly search integers, test the property with trial division,
   and cache found values so that subsequent calls are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static inline bool good(idx_t x)
{
    if (x <= 1) return true;                /* 1 has no divisor >1 */
    idx_t lim = (idx_t) sqrt((long double) x);
    for (idx_t d = 2; d <= lim; ++d)
    {
        if (x % d == 0)
        {
            if (x % (d + 1) == 0) return false;
            idx_t d2 = x / d;
            if (d2 > 1 && d2 != d)
            {
                if (x % (d2 + 1) == 0) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* invalid offset, harmless */
    static seq_t *cache   = NULL;              /* cached sequence */
    static size_t cache_len = 0;               /* number of elements cached */
    static size_t cache_cap = 0;               /* capacity of cache array */
    static idx_t next_val = 1;                 /* next integer to test */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);   /* allocation failed */
        cache[0] = (seq_t)1;                     /* a(1) = 1 */
        cache_len = 1;
        next_val  = 2;
    }

    /* extend cache until we have at least n elements */
    while (cache_len < n)
    {
        if (good(next_val))
        {
            if (cache_len == cache_cap)
            {
                cache_cap <<= 1;                 /* double capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_len] = (seq_t)next_val;
            ++cache_len;
        }
        ++next_val;
    }

    return cache[n - 1];                       /* sequence is 1‑indexed */
}
