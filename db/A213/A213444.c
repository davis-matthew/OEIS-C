/*
 * A213444  Numbers n such that decimal expansion of n^2 contains a 2.
 * https://oeis.org/A213444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer such that the decimal expansion
 *            of n^2 contains a digit 2
 * Range:     u8 n <= 3, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   contains_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_two.h"

/* static cache of numbers that satisfy the condition */
static seq_t *cache   = NULL;   /* array of satisfying numbers */
static size_t cache_len = 0;    /* number of elements stored */
static size_t cache_cap = 0;    /* allocated capacity */
static seq_t cand   = 1;        /* next candidate to test */

/* grow the cache to accommodate at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
        {
            /* Out of memory – aborting is safest */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* fill cache until it contains at least n elements */
static void fill_cache_to(idx_t n)
{
    while (cache_len < n)
    {
        seq_t sq = cand * cand;
        if (contains_two(sq))
        {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
        cand++;
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1 – undefined for 0 */
        return 0;
    fill_cache_to(n);
    return cache[n - 1];
}
