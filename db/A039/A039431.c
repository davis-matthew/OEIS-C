/*
 * A039431  Numbers whose base-10 representation has the same nonzero number
 *          of 5's and 9's.
 * https://oeis.org/A039431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-10 representation has the same nonzero number of 5's and 9's.
   This function searches forward from the smallest such number (59) until it
   finds the n-th term.  The search stops as soon as the count reaches n.  */
seq_t a(idx_t n)
{
    idx_t count = 0;           /* how many numbers we have seen so far */
    seq_t x = 59;              /* the first number with one 5 and one 9 */

    for (;; ++x) {
        /* Count how many 5's and 9's the current number contains */
        int c5 = 0, c9 = 0;
        seq_t y = x;
        while (y > 0) {
            seq_t d = y % 10;
            if (d == 5) ++c5;
            else if (d == 9) ++c9;
            y /= 10;
        }

        if (c5 > 0 && c5 == c9) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
