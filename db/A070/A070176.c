/*
 * A070176  Let s(n) be smallest number >= n which is a sum of two squares
 *          (A001481); sequence gives s(n) - n.
 * https://oeis.org/A070176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = s(n) - n where s(n) is the smallest number ≥ n that can
 *            be written as the sum of two squares. Algorithm: for each
 *            integer i we keep a running pointer cur_m that is the current
 *            candidate for the next sum‑of‑two‑squares. For each new i we
 *            advance cur_m until it satisfies the prime‑factor con...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_sum_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_sum_two_squares.h"

static seq_t *cache = NULL;          /* cache of a(i) values */
static size_t cache_len = 0;         /* number of entries stored */
static size_t cache_alloc = 0;       /* allocated size of cache */
static unsigned long long cur_m = 0; /* current candidate for sum of two squares */

/* Ensure the cache can store at least 'need' entries. */
static void ensure_cache(size_t need)
{
    if (need <= cache_alloc)
        return;
    size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
    if (new_alloc < need)
        new_alloc = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    cache = tmp;
    cache_alloc = new_alloc;
}

/* Compute a(n).  This function may be called in any order. */
seq_t a(idx_t n)
{
    /* Allocate cache lazily. */
    if (cache_len == 0 && cache == NULL)
        ensure_cache(16);

    /* Extend cache up to n. */
    if (n >= cache_len)
    {
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (cur_m < i)   /* start from i if we haven't reached it yet */
                cur_m = i;
            while (!is_sum_two_squares(cur_m))
                ++cur_m;
            cache_len++;            /* increase length before storing */
            ensure_cache(cache_len);
            cache[cache_len - 1] = (seq_t)(cur_m - i);  /* store a(i) */
        }
    }
    return cache[n];
}
