/*
 * A043446  Numbers having two 6's in base 8.
 * https://oeis.org/A043446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6860, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for numbers whose base‑8 representation has exactly two 6’s,
   listed in increasing order. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the offset 1 sequence */
        return (seq_t)0;

    idx_t found = 0;

    for (idx_t cand = 0; ; ++cand)
    {
        seq_t val  = (seq_t)cand;   /* current candidate */
        seq_t tmp  = val;
        idx_t cnt6 = 0;

        while (tmp > 0)
        {
            if ((tmp % 8) == 6)
                ++cnt6;
            tmp /= 8;
        }

        if (cnt6 == 2)
        {
            ++found;
            if (found == n)
                return val;
        }
    }
}
