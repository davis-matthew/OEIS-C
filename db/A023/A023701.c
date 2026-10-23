/*
 * A023701  Numbers with exactly 3 2's in their ternary expansion.
 * https://oeis.org/A023701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer whose base‑3 representation contains
   exactly three digits equal to 2.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* not used, for safety */

    seq_t m = 0;                    /* candidate number */
    idx_t count = 0;                /* how many found so far */

    for (m = 0;; ++m)
    {
        /* Count '2' digits in base 3 of m. */
        seq_t tmp = m;
        idx_t twos = 0;
        while (tmp != (seq_t)0)
        {
            if ((tmp % (seq_t)3) == (seq_t)2)
                ++twos;
            if (twos > 3)            /* early exit */
                break;
            tmp /= (seq_t)3;
        }

        if (twos == 3)
        {
            ++count;
            if (count == n)
                return m;           /* found the n-th term */
        }
    }
}
