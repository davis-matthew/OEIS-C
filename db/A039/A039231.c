/*
 * A039231  Numbers whose base-12 representation has the same number of 2's
 *          and 4's.
 * https://oeis.org/A039231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer (a(1)=0) whose base‑12 *
 *            representation contains the same number of digit 2's and 4's. *
 *            The function performs a direct search and caches results for *
 *            repeated calls
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_twos_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_twos_fours.h"

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* how many slots are allocated */
    static idx_t cache_len = 0;    /* how many terms are stored */

    /* If we already have the term cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Expand the cache to hold at least n terms. */
    if (n > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        cache = tmp;
        cache_cap = n;
    }

    /* Start scanning from the next integer after the last cached one. */
    seq_t val;
    if (cache_len == 0)
    {
        /* First term is 0. */
        cache[0] = (seq_t)0;
        cache_len = 1;
        val = (seq_t)1;
    }
    else
    {
        val = cache[cache_len - 1] + (seq_t)1;
    }

    /* Keep adding qualifying numbers until we have n terms. */
    while (cache_len < n)
    {
        if (has_equal_twos_fours(val))
            cache[cache_len++] = val;
        val = val + (seq_t)1;
    }

    return cache[n - 1];
}
