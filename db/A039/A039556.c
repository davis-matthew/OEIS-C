/*
 * A039556  Numbers whose base-12 representation has the same nonzero number
 *          of 9's and 10's.
 * https://oeis.org/A039556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑12 representation has the
 *            same non‑zero number of digits 9 and 10
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

/* global cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

static void expand_cache(size_t min_cap)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

    /* copy existing entries */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* free old memory */
#ifdef __cplusplus
    delete [] cache;
#else
    free(cache);
#endif

    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 safely */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        ++last_checked;
        seq_t x = (seq_t)last_checked;

        int count9  = 0;
        int count10 = 0;

        /* count digits 9 and 10 in base 12 */
        while (x > (seq_t)0)
        {
            int digit = (int)(x % (seq_t)12);
            if (digit == 9)       ++count9;
            else if (digit == 10) ++count10;
            x /= (seq_t)12;
        }

        if (count9 > 0 && count9 == count10)
        {
            if (cache_len >= cache_cap)
                expand_cache(cache_cap + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
