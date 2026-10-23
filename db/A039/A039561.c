/*
 * A039561  Numbers whose base-4 representation has the number of 0's, 2's and
 *          3's.
 * https://oeis.org/A039561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation has equal numbers of 0's, 2's and 3's. */
seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many terms found so far */
    idx_t val   = 0;      /* candidate number */

    while (count < n)
    {
        val += 1;         /* next integer to test */
        idx_t x = val;

        /* counts of the digits 0, 2 and 3 */
        unsigned c0 = 0, c2 = 0, c3 = 0;

        while (x != 0)
        {
            unsigned d = x & 3u;   /* mod 4 using bitwise AND */
            if (d == 0u) c0 += 1;
            else if (d == 2u) c2 += 1;
            else if (d == 3u) c3 += 1;
            x >>= 2;               /* divide by 4 */
        }

        if (c0 == c2 && c2 == c3)
        {
            count += 1;
            if (count == n)
                return (seq_t)val;
        }
    }

    /* This point is never reached with valid input (n ≥ 1). */
    return (seq_t)0;
}
