/*
 * A039070  Numbers whose base-8 representation has the same number of 3's and
 *          7's.
 * https://oeis.org/A039070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th nonnegative integer whose base‑8 representation
   contains an equal number of digits 3 and 7.  The sequence starts
   0, 1, 2, 4, 5, 6, 8, 9, ...  as given. */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of values found so far   */
    seq_t x = 0;              /* current integer to test         */

    for (;; ++x) {            /* iterate all non‑negative integers */
        seq_t tmp = x;
        unsigned c3 = 0, c7 = 0;   /* counts of digits 3 and 7       */

        /* count 3's and 7's in the base‑8 representation of x */
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 8);
            if (digit == 3) ++c3;
            else if (digit == 7) ++c7;
            tmp /= 8;
        }

        /* x = 0 is treated correctly (c3==c7==0)          */
        if (c3 == c7) {
            ++count;
            if (count == n) return x;   /* nth term found      */
        }
    }
}
