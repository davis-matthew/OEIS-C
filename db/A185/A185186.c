/*
 * A185186  Numbers divisible by at least one of their digits other than 1.
 * https://oeis.org/A185186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates: Numbers divisible by at least one of their decimal digits greater than 1
   (OEIS A185186). The implementation builds the sequence on demand, caching
   previously found terms. */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache      = NULL; /* cached terms */
static size_t  cache_len = 0;    /* number of terms cached */
static size_t  cache_cap = 0;    /* current cache capacity */
static seq_t   next_candidate = 1;/* next integer to test */

/* Grow the cache to accommodate at least 'needed' elements. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache)
        abort(); /* allocation failed */

    if (cache)
    {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure we have at least n terms cached. */
    while (cache_len < (size_t)n)
    {
        seq_t val   = next_candidate;
        seq_t tmp   = val;
        bool good   = false;

        /* Check each decimal digit of val. */
        while (tmp > 0)
        {
            unsigned digit = (unsigned)(tmp % 10);
            if (digit > 1)
            {
                if (val % digit == 0)
                {
                    good = true;
                    break;
                }
            }
            tmp /= 10;
        }

        if (good)
        {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = val;
        }

        next_candidate++;
    }

    return cache[n - 1];
}
