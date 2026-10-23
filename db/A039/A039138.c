/*
 * A039138  Numbers whose base-10 representation has the same number of 3's
 *          and 7's.
 * https://oeis.org/A039138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039138 - Numbers whose decimal representation has the same number of 3's and 7's. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val   = 0;
    while (1)
    {
        seq_t tmp = val;
        int c3 = 0, c7 = 0;
        while (tmp > 0)
        {
            int d = (int)(tmp % 10);
            if (d == 3) c3++;
            else if (d == 7) c7++;
            tmp /= 10;
        }
        if (c3 == c7)
        {
            count++;
            if (count == n)
                return val;
        }
        val++;
    }
}
