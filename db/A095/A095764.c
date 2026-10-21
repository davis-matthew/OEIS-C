/*
 * A095764  Numbers whose name in English contains an "l".
 * https://oeis.org/A095764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = the n‑th positive integer whose English name contains the
 *            letter "l". This implementation finds terms by testing
 *            successive integers, caching found values for repeated calls
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_l_in_3digit, has_l_in_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_l_in_3digit.h"
#include "../utils/has_l_in_number.h"

/* Main sequence function.  It keeps an internal cache so that
   successive calls (in any order) are fast. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache of found terms          */
    static size_t  cap    = 0;      /* capacity of cache             */
    static size_t  len    = 0;      /* current number of terms cached*/
    static idx_t   cur    = 1;      /* next integer to test          */

    /* Grow cache until it contains at least n terms. */
    while (len < n)
    {
        seq_t candidate = (seq_t)cur;

        if (cur < 1000000)  /* test explicitly for numbers < 1e6 */
        {
            if (has_l_in_number(candidate))
            {
                if (len == cap) {
                    cap = (cap == 0) ? 256 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    cache = tmp;
                }
                cache[len++] = candidate;
            }
            ++cur;
        }
        else               /* all numbers ≥ 1e6 contain "million" which has 'l' */
        {
            if (len == cap) {
                cap = (cap == 0) ? 256 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[len++] = candidate;
            ++cur;
        }
    }
    return cache[n - 1];
}
