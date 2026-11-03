/*
 * A043162  Numbers k such that 1 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_adj(seq_t x)
{
    /* No adjacency if only one digit */
    if (x < (seq_t)8)
        return false;

    seq_t prev = x % (seq_t)8;   /* least significant digit */
    x /= (seq_t)8;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)8;   /* next higher digit */
        if ((prev == (seq_t)1 && cur == (seq_t)5) ||
            (prev == (seq_t)5 && cur == (seq_t)1))
            return true;
        prev = cur;
        x /= (seq_t)8;
    }
    return false;
}

/* a(n) = the n‑th integer k such that the base‑8 representation of k
 * contains an adjacent pair of digits 1 and 5 (in either order) while
 * k−1 does not. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    bool prevAdj = false;   /* adjacency status of k‑1, initially false */

    for (idx_t k = 1; ; ++k) {
        seq_t k_val = (seq_t)k;
        bool curAdj = has_adj(k_val);
        if (curAdj && !prevAdj) {
            ++count;
            if (count == n)
                return k_val;
        }
        prevAdj = curAdj;
    }
}
