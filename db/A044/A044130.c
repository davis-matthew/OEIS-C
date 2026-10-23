/*
 * A044130  Numbers n such that string 4,4 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that string 44 occurs in the base-6 representation
   of n but not of n-1. */
/* Implementation: brute‑force search with caching of results. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x in base‑6 contains consecutive digits 4,4. */
static bool has_44(seq_t x)
{
    seq_t prev = 6;           /* sentinel: cannot be 4 */
    while (x > 0)
    {
        seq_t cur = x % 6;
        if (cur == 4 && prev == 4)
            return true;
        prev = cur;
        x /= 6;
    }
    return false;
}

/* Return the n-th term of A044130.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static idx_t cache_cap   = 0;
    static seq_t next_candidate = 1;   /* next number to test */

    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure that the cache can hold 'n' terms. */
    idx_t required = n;
    if (required > cache_cap)
    {
        idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < required)
            new_cap <<= 1;

        void *tmp_mem = realloc((void*)cache, new_cap * sizeof(seq_t));
        if (!tmp_mem)
            abort();
        cache = (seq_t*)tmp_mem;
        cache_cap = new_cap;
    }

    /* Generate additional terms until we have 'n' of them. */
    seq_t x = next_candidate;
    while (cache_len < required)
    {
        if (has_44(x) && !has_44(x - 1))
            cache[cache_len++] = x;
        x++;
    }
    next_candidate = x;   /* remember where we left off */

    return cache[n - 1];
}
