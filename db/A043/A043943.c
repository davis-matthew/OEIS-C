/*
 * A043943  Numbers k such that 1 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequences A043943: numbers k such that the base‑8 representation of k contains
   the adjacent digits 1 and 6 (in either order) but the representation of k+1
   does not.  The function caches results so that calls with arbitrary indices
   are answered in O(n) time, where n is the requested index. */
#include <stdlib.h>

/* Return 1 if x has at least one pair of adjacent 1/6 digits in its octal
   representation, otherwise return 0. */
static int has_adjacent_1_6(seq_t x)
{
    if (x < ((seq_t)8))
        return 0;

    seq_t prev = x % ((seq_t)8);   /* least significant octal digit */
    x     /= ((seq_t)8);
    while (x > 0)
    {
        seq_t cur = x % ((seq_t)8);
        if (   (prev == 1 && cur == 6)
            || (prev == 6 && cur == 1))
            return 1;
        prev = cur;
        x   /= ((seq_t)8);
    }
    return 0;
}

/* seq_t a(idx_t n) – returns the n‑th term of A043943 (a(1)=14). */
seq_t a(idx_t n)
{
    /* behavior for n==0 is irrelevant but returned 0 for safety */
    if (n == 0)
        return 0;

    /* Static cache for the sequence values found so far. */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 1; /* next integer to test for the property */

    while (cache_cnt < (size_t)n)
    {
        seq_t k = next_k;
        if (has_adjacent_1_6(k) && !has_adjacent_1_6(k + 1))
        {
            /* grow the cache if necessary */
            if (cache_cnt == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;           /* allocation failed – unlikely */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
