/*
 * A044660  Numbers n such that string 3,1 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th positive integer whose base‑9 representation
   contains the adjacent digits 3, followed by 1, and such that the base‑9
   representation of that integer plus one does NOT contain 3,1. */
/* The function fills an internal cache – it is correct for any call order
   and runs quickly for n up to ~10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return true if the base‑9 representation of x contains the substring "31". */
static inline bool has31(seq_t x)
{
    seq_t t = x;
    int prev = -1;                     /* previous least significant digit */
    while (t > 0)
    {
        int cur = (int)(t % 9);         /* current least significant digit 0..8 */
        if (cur == 3 && prev == 1)      /* check 3 followed by 1 */
            return true;
        prev = cur;
        t /= 9;
    }
    return false;
}

/* Return the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;    /* largest integer already tested */

    /* Grow the cache if needed. */
    if (cache_len < n)
    {
        if (cache_cap < n)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < n)
                new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
            {
                /* Out of memory: abort the program. */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Continue searching for terms until enough are cached. */
        while (cache_len < n)
        {
            ++last_checked;                    /* next candidate integer */
            seq_t cand = (seq_t)last_checked;
            if (has31(cand))
            {
                seq_t nxt = cand + 1;
                if (!has31(nxt))
                {
                    cache[cache_len++] = cand;
                }
            }
        }
    }

    return cache[n - 1];
}
