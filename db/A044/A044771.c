/*
 * A044771  Numbers n such that string 5,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer m with decimal representation
 *            containing the substring "58", but m+1 does not contain it
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x has the decimal substring 58. */
static bool has58(seq_t x)
{
    /* Scan digits from least significant side.
       We look for a pattern 8 followed by 5 in this reversed order. */
    seq_t prev = x % 10;
    x = x / 10;
    while (x > 0) {
        seq_t cur = x % 10;
        if (prev == 8 && cur == 5)
            return true;
        prev = cur;
        x = x / 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of qualifying integers already seen */
    for (idx_t cand = 1; ; ++cand) {
        seq_t val = cand;
        if (has58(val)) {
            seq_t next = (seq_t)(cand + 1);
            if (!has58(next)) {
                ++found;
                if (found == n)
                    return val;
            }
        }
    }
}
