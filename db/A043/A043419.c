/*
 * A043419  Numbers whose base-7 representation contains exactly three 6's.
 * https://oeis.org/A043419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑7 representation contains exactly three 6's
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: test whether a number has exactly three 6's in base‑7 */
static bool has_three_sixes(seq_t x)
{
    int count = 0;
    while (x > 0)
    {
        if (x % 7 == 6)
        {
            if (++count > 3)
                return false;
        }
        x /= 7;
    }
    return count == 3;
}

/* a(n) returns the n-th positive integer whose base‑7 representation
   contains exactly three 6's (n is 1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n==0, safe guard */

    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* next candidate to test for the property, start at first valid term */
    static seq_t next_candidate = 342;   /* base‑7 666 */

    /* enlarge cache until it contains at least n terms */
    if ((size_t)n > cache_size)
    {
        /* ensure capacity */
        if (cache_cap < cache_size + 64)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* find next terms until we have enough */
        while ((size_t)n > cache_size)
        {
            while (!has_three_sixes(next_candidate))
                ++next_candidate;       /* skip non‑valid numbers */
            cache[cache_size++] = next_candidate;
            ++next_candidate;           /* advance to the next number */
        }
    }

    return cache[n - 1];                /* 1‑based indexing */
}
