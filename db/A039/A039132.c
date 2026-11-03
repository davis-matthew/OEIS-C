/*
 * A039132  Numbers whose base-10 representation has the same number of 2's
 *          and 7's.
 * https://oeis.org/A039132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-10 representation has the same number of 2's
 *            and 7's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if the decimal representation of x contains the same
 * number of digit 2 and digit 7. */
static bool equal_two_sevens(seq_t x)
{
    if (x == (seq_t)0)
        return true;            /* 0 has zero 2's and zero 7's */

    int cnt2 = 0, cnt7 = 0;
    while (x > (seq_t)0)
    {
        int d = (int)(x % (seq_t)10);
        if (d == 2)
            ++cnt2;
        else if (d == 7)
            ++cnt7;
        x /= (seq_t)10;
    }
    return cnt2 == cnt7;
}

/* Static cache for memoisation */
static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap = 0;
static idx_t next_candidate = 0;

/* Make sure that seq_cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL)
    {
        seq_cache_cap = 32;
        seq_cache = mem_new(seq_t, seq_cache_cap);
    }
    while (seq_cache_size < (size_t)n)
    {
        seq_t val = (seq_t)next_candidate;
        if (equal_two_sevens(val))
        {
            if (seq_cache_size >= seq_cache_cap)
            {
                seq_cache_cap *= 2;
                seq_cache = mem_resize(seq_t, seq_cache, seq_cache_cap);
            }
            seq_cache[seq_cache_size++] = val;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined offset, return 0 for safety */

    ensure_cache(n);
    return seq_cache[n - 1];
}
