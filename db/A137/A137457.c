/*
 * A137457  Consider a row of standard dice as a counter. This sequence
 *          enumerates the number of changes (one face rotated over an edge to
 *          an adjacent face) from n-1 to n.
 * https://oeis.org/A137457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let x = n-1. Count t = number of trailing base‑6 digits equal to
 *            5 in x. If x becomes 0 after removing those 5s (i.e. n-1 =
 *            6^k-1), then a(n) = 2*t + 1 otherwise let v be the next digit
 *            (0–4); if v==2 (faces 3→4) the cost is 2, else the cost is 1.
 *            Finally a(n) = 2*t + (v==2 ? 2 : 1). For n = 0, a...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t x = (seq_t)n - 1;
    idx_t t = 0;

    while (x % (seq_t)6 == (seq_t)5) {
        ++t;
        x /= (seq_t)6;
    }

    seq_t base = ((seq_t)t) * 2;   /* 2 * t */

    if (x == (seq_t)0) {
        /* all digits were 5s; add new leading digit 1 */
        return base + 1;
    } else {
        seq_t v = x % (seq_t)6;   /* digit before the trailing 5s, 0..4 */
        seq_t inc = (v == (seq_t)2) ? 2 : 1;
        return base + inc;
    }
}
