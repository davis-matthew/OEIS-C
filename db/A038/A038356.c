/*
 * A038356  Numbers whose base-6 representation has the same nonzero number of
 *          3's and 5's.
 * https://oeis.org/A038356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑6 representation has the same nonzero
   number of 3’s and 5’s (OEIS A038356).  Computed by brute‑force search
   – iterate through integers, test the digits, and return the n‑th
   matching value.  This works for all supported seq_t types and
   satisfies the time bounds for n up to about 10 000. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for offset 1, but harmless */

    seq_t count = 0;
    seq_t value = 1;

    for (;;) {
        seq_t y = value;
        int c3 = 0, c5 = 0;
        while (y) {
            seq_t d = y % (seq_t)6;
            if (d == (seq_t)3) {
                ++c3;
            } else if (d == (seq_t)5) {
                ++c5;
            }
            y /= (seq_t)6;
        }
        if (c3 > 0 && c3 == c5) {
            ++count;
            if (count == n)
                return value;
        }
        ++value;
    }
}
