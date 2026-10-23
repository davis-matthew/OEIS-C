/*
 * A039384  Numbers whose base-9 representation has the same nonzero number of
 *          4's and 6's.
 * https://oeis.org/A039384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers whose base‑9 representation has the same nonzero
 *            number of 4's and 6's
 * Range:     u8 n <= 6, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t val = 1;
    while (found < n) {
        seq_t tmp = val;
        unsigned int c4 = 0, c6 = 0;
        while (tmp > 0) {
            seq_t digit = tmp % 9;
            if (digit == 4) c4++;
            else if (digit == 6) c6++;
            tmp /= 9;
        }
        if (c4 == c6 && c4 > 0) {
            found++;
            if (found == n)
                return val;
        }
        val++;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
