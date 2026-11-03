/*
 * A044296  Numbers n such that string 5,0 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n whose base‑9 representation contains
 *            the adjacent digits 5,0 and that property does not hold for n-1
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑9 representation of x contains the substring
   5,0 (digits 5 followed by 0 when reading from most significant to
   least significant).  The test is performed by scanning the digits
   from least significant to most significant: a pair 0 followed by 5
   in this order corresponds to 5,0 in the normal representation. */
static bool has_50_seq9(seq_t x)
{
    bool prev0 = false;           /* was the previous (less significant)
                                     digit 0?  */
    while (x != (seq_t)0)
    {
        unsigned digit = (unsigned)(x % (seq_t)9);
        x /= (seq_t)9;

        if (prev0 && digit == 5)
            return true;

        prev0 = (digit == 0);
    }
    return false;
}

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Cache of already found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = (seq_t)1;   /* next integer to test */

    /* Generate new terms until we have at least n of them. */
    while (cache_len < n)
    {
        seq_t cand = next_candidate++;
        seq_t cand_minus_one = cand - (seq_t)1;

        if (has_50_seq9(cand) && !has_50_seq9(cand_minus_one))
        {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                /* If realloc fails, the program will terminate; this is
                   acceptable for the purposes of this problem. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
