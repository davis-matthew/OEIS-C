/*
 * A039369  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 6's.
 * https://oeis.org/A039369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-9 representation has the same nonzero number
 *            of 1's and 6's. The implementation searches sequentially from 1
 *            upwards until it has found the requested n‑th term
 * Range:     u8 n <= 18, u16 n <= 8513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m holds the candidate numbers */
    seq_t m = 0;
    /* found counts already selected numbers */
    idx_t found = 0;

    while (found < n) {
        /* next candidate */
        m = m + 1;

        /* count 1's and 6's in base‑9 */
        int c1 = 0, c6 = 0;
        seq_t x = m;
        while (x != 0) {
            int digit = (int)(x % 9);
            if (digit == 1)
                c1++;
            else if (digit == 6)
                c6++;
            x = x / 9;
        }

        if (c1 == c6 && c1 > 0)
            found++;
    }

    return m;
}
