/*
 * A043226  Numbers k such that 1 and 3 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has13or31(seq_t x)
{
    int prev = 10;               /* sentinel – not a decimal digit */
    while (x > 0)
    {
        int d = (int)(x % 10);   /* least‑significant digit */
        if ((prev == 1 && d == 3) || (prev == 3 && d == 1))
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

/* a(n) = the nth integer k such that the decimal representation of k contains
   "13" or "31", but the representation of k-1 does not. */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is unused */
        return 0;

    static seq_t  *seq = NULL;
    static size_t  len  = 0;
    static size_t  cap  = 0;
    static idx_t   last_checked = 12;      /* 13 is the first candidate to test */

    while (len < (size_t)n)
    {
        ++last_checked;                     /* next candidate number */
        seq_t k = (seq_t)last_checked;

        if (has13or31(k) && !has13or31(k - 1))
        {
            if (len == cap)
            {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                seq = tmp;
                cap = new_cap;
            }
            seq[len++] = k;                  /* store term */
        }
    }
    return seq[n - 1];
}
