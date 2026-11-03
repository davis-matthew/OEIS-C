/*
 * A043130  Numbers k such that 4 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043130: numbers k such that the base‑6 representation of k
   contains the adjacent digits 4 and 5 in either order, but k−1 does not.
   The function returns the n‑th such number (1‑based index). */

#include <stdbool.h>

/* Return true iff the base‑6 representation of k contains
   either the substring "45" or the substring "54". */
static bool has_adj_base6(idx_t k)
{
    if (k < 6) return false;
    idx_t prev = k % 6;
    k /= 6;
    while (k)
    {
        idx_t cur = k % 6;
        if ((prev == 4 && cur == 5) || (prev == 5 && cur == 4))
            return true;
        prev = cur;
        k /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index 0 is unused */

    idx_t count = 0;               /* number of terms found so far */
    idx_t k = 1;                   /* candidate numbers to test */

    while (1)
    {
        bool cur_adj  = has_adj_base6(k);
        bool prev_adj = k > 0 ? has_adj_base6(k - 1) : false;

        if (cur_adj && !prev_adj)
        {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
