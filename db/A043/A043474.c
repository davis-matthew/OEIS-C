/*
 * A043474  Numbers having two 5's in base 9.
 * https://oeis.org/A043474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers having exactly two 5's in base 9. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number contains exactly two digits '5' in base 9. */
static bool has_two_fives(seq_t x)
{
    unsigned count = 0;
    seq_t y = x;
    while (y != (seq_t)0)
    {
        seq_t d = y % (seq_t)9;
        if (d == (seq_t)5)
        {
            ++count;
            if (count > 2) return false;
        }
        y = y / (seq_t)9;
    }
    return count == 2;
}

/* Global cache for previously found terms */
static seq_t *cache   = NULL;   /* array of found terms */
static size_t   cap   = 0;      /* allocated capacity of cache */
static idx_t    sz    = 0;      /* number of terms currently cached */
static seq_t   next_val = (seq_t)0;   /* next integer to test for the property */

/* Ensure the cache has room for at least 'need' terms */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, newcap);
    cache = tmp;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    /* Generate terms up to the requested index if necessary */
    while (sz < n)
    {
        /* Find next integer with two 5's in base 9 */
        while (!has_two_fives(next_val))
            ++next_val;

        /* Store the found term */
        ensure_capacity(sz + 1);
        cache[sz++] = next_val;
        ++next_val;                       /* advance for the next search */
    }
    return cache[n - 1];
}
