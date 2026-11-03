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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return true if a 3‑digit number (0 ≤ x < 1000) has a word containing
   the letter 'l' in its English name.  Only the words "eleven" and
   "twelve" contain 'l', so we check for those values. */
static bool has_l_in_3digit(int x)
{
    if (x < 20)
        return x == 11 || x == 12;          /* "eleven", "twelve" */
    if (x < 100)
        return false;                       /* tens have no "l" */
    /* For numbers ≥ 100 check the remainder < 100 (e.g., 113 → 13) */
    return has_l_in_3digit(x % 100);
}

/* Return true if a number < 1 000 000 has an English name containing 'l'.
   The number is split into thousands and lower parts; if either part
   contains 'l', the whole number contains 'l'. */
static bool has_l_in_number(seq_t val)
{
    int thousands = (int)(val / 1000) % 1000;
    if (thousands > 0 && has_l_in_3digit(thousands))
        return true;
    int lower = (int)(val % 1000);
    if (lower > 0 && has_l_in_3digit(lower))
        return true;
    return false;
}

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
                    seq_t *tmp = mem_resize(seq_t, cache, cap);
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
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                cache = tmp;
            }
            cache[len++] = candidate;
            ++cur;
        }
    }
    return cache[n - 1];
}
