/*
 * A045287  Numbers whose base-5 representation contains exactly two 2's and
 *          two 4's.
 * https://oeis.org/A045287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base-5 representation contains exactly two 2's and
 *            two 4's
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th positive integer whose base‑5 representation
   has exactly two digits equal to 2 and exactly two digits equal to 4.
   n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 0;

    while (true) {
        /* Count 2’s and 4’s in the base‑5 expansion of num. */
        seq_t tmp = num;
        idx_t c2 = 0, c4 = 0;
        while (tmp > 0) {
            seq_t d = tmp % (seq_t)5;
            tmp = tmp / (seq_t)5;
            if (d == (seq_t)2)  c2++;
            else if (d == (seq_t)4) c4++;
        }
        if (c2 == 2 && c4 == 2) {
            count++;
            if (count == n) {
                return num;
            }
        }
        num++;
    }
}
