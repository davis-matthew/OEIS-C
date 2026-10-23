/*
 * A044747  Numbers n such that string 3,4 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal representation of n contains
   the substring "34" but the representation of n+1 does not.  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

static bool has34(seq_t x)
{
    seq_t prev = 10;  /* any digit greater than 9 */
    while (x > 0)
    {
        seq_t d = x % 10;
        if (d == 3 && prev == 4)
            return true;
        prev = d;
        x = x / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cap   = 0;         /* allocated capacity */
    static idx_t  found = 0;         /* number of terms stored */
    static seq_t  m     = 0;         /* next integer to test */

    /* gather terms until we have at least n of them */
    while (found < n)
    {
        ++m;                          /* examine next integer */

        if (has34(m) && !has34(m + 1))
        {
            if (cap <= found)
            {
                size_t newcap = cap ? cap << 1 : 16;
                seq_t *tmp = seq_realloc(cache, cap, newcap);
                if (!tmp)
                    abort();           /* memory allocation failed */
                cache = tmp;
                cap   = newcap;
            }
            cache[found] = m;
            ++found;
        }
    }

    return cache[n - 1];
}
