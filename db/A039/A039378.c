/*
 * A039378  Numbers whose base-9 representation has the same nonzero number of
 *          3's and 4's.
 * https://oeis.org/A039378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n-th number in A039378:
 * numbers whose base‑9 representation has an equal nonzero number of 3’s and 4’s.
 *
 * The function scans successive integers starting from 1, counts
 * the digits 3 and 4 in each base‑9 representation, and
 * returns the one that is the nth such number.
 */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t value = 1;

    for (;; ++value) {
        unsigned int c3 = 0, c4 = 0;
        seq_t x = value;

        /* Count 3’s and 4’s in base‑9 representation. */
        while (x > 0) {
            int d = (int)(x % 9);   /* digit in 0..8 */
            if (d == 3) ++c3;
            else if (d == 4) ++c4;
            x /= 9;
        }

        if (c3 == c4 && c3 != 0) {  /* equal nonzero counts */
            ++found;
            if (found == n)
                return value;
        }
    }
}
