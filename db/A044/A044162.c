/*
 * A044162  Numbers n such that string 3,3 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A044162: nth integer whose base‑7 representation
   contains the consecutive digits “33”, while the preceding integer
   does not.  The function scans through natural numbers in order
   and caches the results, so any order of calls is handled correctly. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has33(seq_t x)
{
    unsigned prev = 0;
    while (x)
    {
        unsigned cur = (unsigned)(x % 7);
        if (prev == 3 && cur == 3)
            return true;
        prev = cur;
        x /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static idx_t  terms_cnt = 0;
    static seq_t  last_checked = 0;

    if (n == 0)
        return 0; /* undefined for n==0, but return 0 safely */

    while (terms_cnt < n)
    {
        /* find next candidate */
        for (;;)
        {
            ++last_checked;
            if (has33(last_checked) && !has33(last_checked - 1))
            {
                if (terms_cnt == (idx_t)terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                    terms = mem_resize(seq_t, terms, new_cap);
                    terms_cap = new_cap;
                }
                terms[terms_cnt++] = last_checked;
                break;
            }
        }
    }
    return terms[n - 1];
}
