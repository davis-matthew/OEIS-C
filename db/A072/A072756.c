/*
 * A072756  a(n) = least nonnegative integer not a(k) and not
 *          a(k)+floor(a(k)/2) for k<n.
 * https://oeis.org/A072756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072756: Least nonnegative integer not equal to any previous a(k)
   nor to a(k)+floor(a(k)/2) for k<n.  Computed by incremental search with
   memoization. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Extend an array of seq_t to at least NewSize elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Extend a byte array used as a bitmap, initializing new part to zero. */
static void grow_used(unsigned char **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    *arr = (unsigned char *)realloc(*arr, new_cap);
    for (size_t i = *cap; i < new_cap; ++i) (*arr)[i] = 0;
    *cap = new_cap;
}

/* Computes a(n).  Works for any idx_t (uint64_t) and any seq_t (uint8_t … __int128 or big). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static unsigned char *used = NULL;
    static size_t used_cap = 0;

    /* Ensure cache big enough. */
    grow_seq(&cache, &cache_cap, (size_t)n + 1);

    if (n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Build sequence up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* Find the smallest unused candidate. */
        seq_t cand = 0;
        for (;; ++cand)
        {
            size_t idx = (size_t)cand;
            if (idx >= used_cap) grow_used(&used, &used_cap, idx + 1);
            if (!used[idx]) break;
        }

        /* Store the candidate as the next term. */
        cache[(size_t)i] = cand;

        /* Mark cand as used. */
        {
            size_t idx = (size_t)cand;
            if (idx >= used_cap) grow_used(&used, &used_cap, idx + 1);
            used[idx] = 1;
        }

        /* Mark cand + floor(cand/2) as used. */
        seq_t off = cand + (cand / 2);
        {
            size_t idx = (size_t)off;
            if (idx >= used_cap) grow_used(&used, &used_cap, idx + 1);
            used[idx] = 1;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
