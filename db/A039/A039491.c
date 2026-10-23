/*
 * A039491  Numbers whose base-11 representation has the same nonzero number
 *          of 8's and 10's.
 * https://oeis.org/A039491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-11 representation has the same nonzero *
 *            number of 8's and 10's
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t  cache_count = 0;
static idx_t  cache_cap   = 0;
static seq_t  next_candidate = 1;

static void grow_cache(idx_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < min_cap)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
#else
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        return; /* allocation failed – caller will ignore */

    if (cache) {
#ifdef __cplusplus
        for (idx_t i = 0; i < cache_count; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        for (idx_t i = 0; i < cache_count; ++i)
            tmp[i] = cache[i];
        free(cache);
#endif
    }

    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A039491 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined – sentinel value */

    /* Generate terms until we have at least n of them */
    while (cache_count < n) {
        seq_t x = next_candidate;
        idx_t  cnt8  = 0;
        idx_t  cnt10 = 0;
        seq_t y = x;
        while (y != 0) {
            seq_t r = y % 11;
            if (r == 8)
                ++cnt8;
            else if (r == 10)
                ++cnt10;
            y /= 11;
        }
        if (cnt8 == cnt10 && cnt8 > 0) {
            grow_cache(cache_count + 1);
            cache[cache_count++] = x;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
