/*
 * A039218  Numbers whose base-12 representation has the same number of 0's
 *          and 9's.
 * https://oeis.org/A039218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th natural number whose base‑12 representation
   contains an equal number of digits 0 and 9.  The sequence starts
   with a(1) = 1.  This implementation searches numbers in increasing
   order until the requested index is reached. */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the specification of the sequence. */
    idx_t found = 0;
    seq_t x = 1;                /* candidate value */

    for (;; ++x) {
        seq_t y = x;
        unsigned zeros = 0, nines = 0;

        /* Count zeros and nines in base‑12. */
        while (y > 0) {
            unsigned d = (unsigned)(y % 12);
            if (d == 0) ++zeros;
            else if (d == 9) ++nines;
            y /= 12;
        }

        if (zeros == nines) {
            ++found;
            if (found == n) return x;
        }
    }
}
