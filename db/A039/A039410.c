/*
 * A039410  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 3's.
 * https://oeis.org/A039410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            has * an equal nonzero number of digits 2 and 3. This
 *            implementation enumerates * all integers in ascending order and
 *            selects the required ones
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* candidate number */
    idx_t count = 0;        /* how many valid numbers seen so far */

    for (;; ++val)
    {
        seq_t tmp = val;
        int cnt2 = 0, cnt3 = 0;

        /* count digits 2 and 3 */
        while (tmp > 0)
        {
            int d = (int)(tmp % 10);
            if (d == 2) ++cnt2;
            else if (d == 3) ++cnt3;
            tmp /= 10;
        }

        if (cnt2 == cnt3 && cnt2 > 0)
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
