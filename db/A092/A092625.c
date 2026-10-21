/*
 * A092625  Numbers with exactly three prime digits.
 * https://oeis.org/A092625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: returns the n‑th positive integer that
 * contains exactly three prime digits (2, 3, 5, or 7).  The function
 * iterates successive integers, counts prime digits in each, and
 * returns the number when the count reaches n.  This method is fast
 * enough for n up to at least 10 000. */
seq_t a(idx_t n)
{
    idx_t   idx   = 0;      /* how many qualifying numbers found   */
    seq_t   num   = 222;    /* first number that can have 3 prime digits */

    for (;; ++num)
    {
        idx_t  prime_cnt = 0;
        seq_t  tmp      = num;

        /* count prime digits while early exiting if more than 3 */
        while (tmp > 0)
        {
            int d = (int)(tmp % 10);
            if (d == 2 || d == 3 || d == 5 || d == 7)
            {
                ++prime_cnt;
                if (prime_cnt > 3) break;
            }
            tmp /= 10;
        }

        if (prime_cnt == 3)
        {
            ++idx;
            if (idx == n)
                return num;
        }
    }
}
