/*
 * A039483  Numbers whose base-11 representation has the same nonzero number
 *          of 6's and 7's.
 * https://oeis.org/A039483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑11 representation has equal
   nonzero counts of digits 6 and 7. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence, safe fallback */

    idx_t count = 0;                /* how many valid numbers found   */
    seq_t  value = 1;               /* current number to test         */

    for (;;)
    {
        seq_t t = value;
        unsigned c6 = 0, c7 = 0;    /* counts of 6’s and 7’s          */

        /* examine base‑11 digits of t */
        while (t > 0)
        {
            unsigned d = (unsigned)(t % 11);
            if (d == 6) ++c6;
            else if (d == 7) ++c7;
            t = t / 11;
        }

        /* is t a valid member of the sequence? */
        if (c6 > 0 && c6 == c7)
        {
            ++count;
            if (count == n)          /* found the n‑th term             */
                return value;
        }

        ++value;                     /* advance to next integer          */
    }
}
