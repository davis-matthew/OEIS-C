/*
 * A045307  Numbers whose base-5 representation contains exactly three 3's and
 *          three 4's.
 * https://oeis.org/A045307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose base‑5 representation contains exactly
 *            three 3's and exactly three 4's
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_three_three_four(seq_t x)
{
    int c3 = 0, c4 = 0;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)5);
        if (d == 3) {
            if (++c3 > 3) return false;
        } else if (d == 4) {
            if (++c4 > 3) return false;
        }
        x /= (seq_t)5;
    }
    return c3 == 3 && c4 == 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    idx_t found = 0;
    seq_t cand = 1;

    for (;;) {
        if (has_three_three_four(cand)) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }
}
