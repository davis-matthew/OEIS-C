/*
 * A044627  Numbers n such that string 7,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑8 representation of n contains
 * the substring “73” but the base‑8 representation of n+1 does not.
 * The function a(idx_t n) returns the n‑th such number (1‑based). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Check whether an idx_t value has the substring 73 in base 8. */
static bool has73(idx_t value)
{
    /* Need at least two digits. */
    if (value < 8u)
        return false;

    idx_t prev = value % 8u;   /* least significant digit */
    value /= 8u;
    while (value)
    {
        idx_t d = value % 8u;   /* next higher digit */
        if (prev == 3u && d == 7u)
            return true;       /* 7 followed by 3 in normal order */
        prev = d;
        value /= 8u;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n==0 is outside the OEIS offset; just return 0 for safety. */
    if (n == 0)
        return 0;

#if defined(__cplusplus)
    /* C++ implementation uses a static vector for automatic
     * construction and destruction of arbitrary‑precision objects. */
    static std::vector<seq_t> cache;
    static idx_t candidate = 0;   /* last examined number */

    /* Generate terms until we have at least n of them. */
    while ((idx_t)cache.size() < n)
    {
        /* Find next number with substring 73 where n+1 does not. */
        do
        {
            ++candidate;
        } while (!(has73(candidate) && !has73(candidate + 1)));

        cache.push_back(static_cast<seq_t>(candidate));
    }
    return cache[n - 1];
#else
    /* C implementation – static POD array; works for unsigned integer
     * seq_t types. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t  candidate  = 0;

    /* First call: allocate initial cache. */
    if (cache == NULL)
    {
        cache_cap = 256;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        /* Find next qualifying number. */
        do
        {
            ++candidate;
        } while (!(has73(candidate) && !has73(candidate + 1)));

        /* Grow cache if needed. */
        if (cache_len >= cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)candidate;
    }

    return cache[n - 1];
#endif
}
