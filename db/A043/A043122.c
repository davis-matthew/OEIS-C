/*
 * A043122  Numbers k such that 1 and 3 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043122: numbers k with the digits 1 and 3 appearing adjacent in base‑6,
   and k–1 not containing such an adjacency.  This is computed by a direct
   brute‑force scan of integers. */
#include <stdbool.h>

/* Return true iff x (in base‑6) contains a pair of adjacent digits that are
   either 1 followed by 3 or 3 followed by 1.  The function works for any
   unsigned integer type seq_t that supports % and / by 64‑bit constants. */
static bool has_adj13_or31(seq_t x)
{
    /* Scan the representation digit by digit. */
    while (x >= 6) {
        seq_t low  = x % 6;          /* least‑significant digit */
        seq_t high = (x / 6) % 6;    /* the next more significant digit */
        if ((low == 1 && high == 3) || (low == 3 && high == 1))
            return true;
        x = x / 6;                  /* advance to the next pair */
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t count = 0;  /* how many qualifying k's we have found */
    seq_t k = 1;      /* current integer to test */

    while (1) {
        if (has_adj13_or31(k) && !has_adj13_or31(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
