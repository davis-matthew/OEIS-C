/*
 * A045174  Numbers whose base-5 representation contains no 0's and exactly
 *          two 2's.
 * https://oeis.org/A045174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of integers whose base‑5 representation
   contains no digit 0 and exactly two digits 2.  This routine
   generates the numbers in ascending order and returns the n‑th
   element (a(1) is the first such integer). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* power = 5^len, start from len = 1. */
    seq_t power = 1;          /* 5^0 */
    idx_t  found = 0;         /* how many terms we have found */

    const seq_t five = 5;     /* use a seq_t constant for division */

    for (idx_t len = 1;; ++len) {
        power = power * five;               /* now power = 5^len */

        if (len < 2) continue;              /* need at least two digits */

        seq_t low  = power / five;          /* 5^(len-1) */
        seq_t high = power - 1;              /* 5^len - 1 */

        for (seq_t val = low; val <= high; val = val + 1) {
            seq_t tmp  = val;
            idx_t  cnt2 = 0;
            bool   bad  = false;

            while (tmp != 0) {
                seq_t d = tmp % five;
                if (d == 0) { bad = true; break; }
                if (d == 2) cnt2++;
                tmp = tmp / five;
            }

            if (!bad && cnt2 == 2) {
                ++found;
                if (found == n) return val;
            }
        }
    }

    /* should never reach here */
    return 0;
}
