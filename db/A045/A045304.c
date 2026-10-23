/*
 * A045304  Numbers whose base-5 representation contains exactly three 3's and
 *          no 4's.
 * https://oeis.org/A045304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A045304: numbers whose base‑5
 * representation contains exactly three 3's and no 4's.
 * The function searches integers in ascending order, checks the
 * required digit pattern, and returns the nth such number
 * (with a(1) = 93). */
seq_t a(idx_t n)
{
    if (n == 0) /* behaviour below the offset is undefined */
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t x = 0; ; ++x)
    {
        idx_t t   = x;
        int   c3  = 0;
        int   bad = 0;

        /* Inspect base‑5 digits of x */
        while (t > 0)
        {
            int d = t % 5;
            t   /= 5;

            if (d == 3)
                ++c3;
            else if (d == 4) /* digit 4 is not allowed */
            {
                bad = 1;
                break;
            }
        }

        if (!bad && c3 == 3)    /* x meets the criteria */
        {
            ++count;
            if (count == n)
                return (seq_t)x;   /* found the nth term */
        }
    }
}
