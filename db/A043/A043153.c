/*
 * A043153  Numbers k such that 0 and 2 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A043153: numbers k such that the base‑8 representation of k contains
   the adjacent pattern 0 and 2 (either '02' or '20') and the representation of k‑1
   does not. */
#include <stdbool.h>

/* Return true if x's octal digits contain a 0 next to a 2, in either order. */
static bool has_adj_oct(seq_t x)
{
    if (x == 0) return false;
    seq_t temp = x;
    int prev = -1;
    while (temp > 0) {
        int d = (int)(temp % 8);
        if (prev != -1 && ((prev == 0 && d == 2) || (prev == 2 && d == 0)))
            return true;
        prev = d;
        temp /= 8;
    }
    return false;
}

/* Return the n‑th element of the sequence. */
seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t k   = 1;
    bool prev_adj = false;               /* adjacency status of k‑1 */

    while (1) {
        bool cur_adj = has_adj_oct(k);
        if (cur_adj && !prev_adj) {
            ++cnt;
            if (cnt == n)
                return k;
        }
        prev_adj = cur_adj;
        ++k;
    }
}
