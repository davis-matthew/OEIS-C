/*
 * A043109  Numbers k such that 0 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043109:  numbers k such that 0 and 4 occur juxtaposed in the base‑5 representation of k,
    but not in that of k‑1.
    The function counts such numbers starting from k = 1 and returns the n‑th one.
*/

#include <stdbool.h>

static bool has_adj_04(seq_t x)
{
    seq_t tmp = x;
    seq_t prev = 0;
    bool first = true;
    while (tmp > 0) {
        seq_t digit = tmp % 5;
        tmp /= 5;
        if (!first) {
            if ((prev == 0 && digit == 4) || (prev == 4 && digit == 0))
                return true;
        }
        first = false;
        prev = digit;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    bool prevAdj = false;    /* adjacency for k‑1 (initially for 0, which is false) */

    while (1) {
        bool currAdj = has_adj_04(k);
        if (currAdj && !prevAdj) {
            ++count;
            if (count == n)
                return k;
        }
        prevAdj = currAdj;
        ++k;
    }
}
