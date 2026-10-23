/*
 * A039601  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 9's.
 * https://oeis.org/A039601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑12 representation contains the same
   non‑zero number of digits 0 and 9.  The sequence is defined by
   a(n) = the n‑th positive integer v such that in base 12
   count₀(v) = count₉(v) > 0.  */
#include <stdlib.h>
#include <stdbool.h>

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Static cache of the sequence values for repeated calls. */
    static seq_t *cache = NULL;   /* array of computed terms      */
    static size_t  count = 0;     /* number of terms stored       */
    static size_t  capacity = 0;  /* allocated capacity           */
    static seq_t   last_val = 0;  /* last integer examined        */

    /* Fast return for invalid index. */
    if (n == 0) return 0;

    /* Ensure the cache has at least n terms. */
    while (count < n)
    {
        /* Search for the next qualifying number. */
        for (seq_t candidate = last_val + 1; count < n; ++candidate)
        {
            seq_t temp = candidate;
            unsigned int cnt0 = 0, cnt9 = 0;
            /* Count digits in base 12. */
            while (temp > 0)
            {
                unsigned int dig = (unsigned int)(temp % 12);   /* 0 … 11 */
                temp  = temp / 12;
                if (dig == 0) ++cnt0;
                else if (dig == 9) ++cnt9;
            }
            if (cnt0 > 0 && cnt0 == cnt9)
            {
                /* Store the term.  Grow the cache if needed. */
                if (count == capacity)
                {
                    size_t new_cap = capacity ? capacity * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();        /* allocation failed */
                    cache = tmp;
                    capacity = new_cap;
                }
                cache[count++] = candidate;
                last_val = candidate;
            }
        }
    }

    /* Return the requested term. */
    return cache[n - 1];
}
