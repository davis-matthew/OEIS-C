/*
 * A043863  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 5 mod 9.
 * https://oeis.org/A043863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the number of runs in the base‑4 representation
   of n is congruent to 5 (mod 9).  The function a(n) returns the
   n‑th such number (a(1) = 273). */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of discovered terms      */
static size_t cache_len = 0;       /* number of cached terms          */
static size_t cache_cap = 0;       /* capacity of cache              */
static seq_t next_candidate = 1;   /* next integer to test           */

/* Count runs in the base‑4 representation of x.  */
static inline int count_runs(seq_t x)
{
    seq_t t = x;
    int prev = -1;
    int runs = 0;
    while (t > 0)
    {
        int d = (int)(t % 4);
        if (d != prev)
        {
            runs++;
            prev = d;
        }
        t /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    /* Ensure the sequence is computed up to index n.  */
    while (cache_len < n)
    {
        /* Test the current candidate.  */
        if (count_runs(next_candidate) % 9 == 5)
        {
            /* Grow the cache if needed.  */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)
                    return 0;   /* allocation failure; caller may handle */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len] = next_candidate;
            cache_len++;
        }
        /* Advance to the next integer.  */
        next_candidate++;
    }
    return cache[n - 1];
}
