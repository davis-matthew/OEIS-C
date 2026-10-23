/*
 * A039234  Numbers whose base-12 representation has the same number of 2's
 *          and 7's.
 * https://oeis.org/A039234
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

/* Sequence A039234: numbers whose base‑12 representation has equal
   number of 2's and 7's – found by brute‑force enumeration. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 0;
    for (;; ++cand)
    {
        seq_t tmp = cand;
        unsigned c2 = 0, c7 = 0;

        if (tmp == 0)
        {
            /* representation "0" has 0 twos and 0 sevens */
        }
        else
        {
            while (tmp > 0)
            {
                seq_t d = tmp % 12;
                if (d == 2u)
                    ++c2;
                else if (d == 7u)
                    ++c7;
                tmp /= 12;
            }
        }

        if (c2 == c7)
        {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
