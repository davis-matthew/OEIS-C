/*
 * A043948  Numbers k such that 2 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that the base‑8
 *            representation of k contains an adjacent pair of digits 2 and 6
 *            (in either order) while the base‑8 representation of k+1 does
 *            not. The sequence is obtained by a simple search
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether the base‑8 representation of |x| contains an
   adjacent pair of digits 2 and 6 (either 26 or 62). */
static inline bool has_adj_26(seq_t x)
{
    /* Use a sentinel value outside the range 0–7 for the previous digit. */
    const seq_t sentinel = (seq_t)16;
    seq_t prev = sentinel;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)8;
        if ((prev == (seq_t)2 && d == (seq_t)6) ||
            (prev == (seq_t)6 && d == (seq_t)2))
            return true;
        prev = d;
        x /= (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Find the n‑th number satisfying the condition. */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (has_adj_26((seq_t)k) && !has_adj_26((seq_t)(k + 1))) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
