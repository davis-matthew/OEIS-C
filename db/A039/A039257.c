/*
 * A039257  Numbers whose base-12 representation has the same number of 5's
 *          and 9's.
 * https://oeis.org/A039257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose base‑12 representation
 *            contains the same number of digits 5 as digits 9. a(1)=0,
 *            a(2)=1,
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset but harmless */

    idx_t found = 0;
    seq_t x = 0;

    while (found < n)
    {
        /* Count 5's and 9's in the base‑12 representation of x. */
        seq_t t     = x;
        int    diff = 0;   /* diff = (#5) – (#9) */

        while (t > 0)
        {
            seq_t d = t % (seq_t)12;
            if (d == 5)
                diff++;
            else if (d == 9)
                diff--;
            t = t / (seq_t)12;
        }

        if (diff == 0)
        {
            found++;
            if (found == n)
                return x;
        }
        x++;
    }

    /* Should never happen. */
    return x;
}
