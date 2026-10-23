/*
 * A044569  Numbers n such that string 0,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A044569
   Description: a(n) is the n-th integer whose base‑8 representation contains the substring "01" 
   but the next integer n+1's base‑8 representation does NOT contain "01".
*/

#include <stdlib.h>
#include <stdbool.h>

/* check whether a base‑8 number contains the consecutive digits 0 then 1 (from most significant to least) */
static bool has01(seq_t x)
{
    seq_t prev = (seq_t)-1;             /* sentinel value that is never 1 */
    while (x > 0)
    {
        seq_t digit = x % (seq_t)8;
        if (digit == 0 && prev == 1)
            return true;
        prev = digit;
        x /= (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1, but harmless */

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_len = 0;     /* number of valid elements in cache */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t next_candidate = 1; /* next integer to test */

    /* grow cache until we have at least n terms */
    if (cache_len < n)
    {
        /* grow capacity if necessary */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            if (new_cap < (size_t)n)
                new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* allocation failure – return 0 as a safe fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* generate terms until cache_len reaches n */
        while (cache_len < n)
        {
            seq_t cand = (seq_t)next_candidate;
            if (has01(cand) && !has01(cand + (seq_t)1))
            {
                cache[cache_len++] = cand;
            }
            next_candidate++;
        }
    }

    return cache[n - 1];
}
