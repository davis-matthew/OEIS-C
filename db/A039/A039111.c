/*
 * A039111  Numbers whose base-9 representation has the same number of 7's and
 *          8's.
 * https://oeis.org/A039111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Numbers whose base-9 representation has the same number of 7's and 8's.
 * Computed by brute‑force enumeration.  a(1) = 0, a(2) = 1, …
 */
seq_t a(idx_t n)
{
    idx_t count = 0;               /* how many qualifying numbers found */
    seq_t  val   = 0;               /* candidate number to test */

    for (;;)                           /* loop until we hit the n‑th term */
    {
        /* count the difference between number of 7’s and 8’s */
        int diff = 0;
        seq_t tmp = val;
        while (tmp > 0)
        {
            seq_t d = tmp % 9;          /* current base‑9 digit */
            if (d == 7ULL)
                diff++;
            else if (d == 8ULL)
                diff--;
            tmp /= 9;
        }

        if (diff == 0)                   /* qualifies */
        {
            count++;
            if (count == n)               /* found the n‑th term */
                return val;
        }

        val += 1;                        /* next candidate */
    }
}
