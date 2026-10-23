/*
 * A044459  Numbers n such that string 0,1 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A044459:
   numbers n such that the base‑5 representation of n contains
   the substring "01" but the base‑5 representation of n+1 does not.
   The function builds a memoized list of such numbers by scanning
   successive integers and adding those that satisfy the condition. */

#include <stdlib.h>

static inline int has01(seq_t n)
{
    seq_t tmp = n;
    int prev = -1;   /* -1 means no previous digit yet */
    while (tmp)
    {
        /* Least significant base‑5 digit */
        int d = (int)(tmp % 5);
        if (prev == 1 && d == 0)
            return 1;      /* found the pattern “01” */
        prev = d;
        tmp = tmp / 5;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* position 0 is unused */

    static seq_t *cache = NULL;        /* cached terms */
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static seq_t next_start = 0;       /* next candidate number */

    /* Expand cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        seq_t m = next_start;
        /* Find next valid number */
        while (1)
        {
            if (has01(m) && !has01(m + 1))
                break;
            ++m;
        }

        /* Ensure capacity */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = m;
        next_start = m + 1;
    }

    return cache[n - 1];
}
