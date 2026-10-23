/*
 * A043127  Numbers k such that 2 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the base‑6
 *            representation of k contains the adjacent pair 25 or 52, while
 *            k‑1 does not
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t curr = 0;               /* current integer being tested   */
    int   last_has = 0;           /* whether k‑1 contained a pair    */
    idx_t count = 0;              /* how many qualifying numbers found */

    for (;;)
    {
        curr = curr + 1;          /* move to next integer */

        /* test whether curr contains 25 or 52 in base‑6 */
        seq_t tmp = curr;
        int   prev_digit = -1;
        int   has_pair   = 0;

        while (tmp != 0)
        {
            int cd = (int)(tmp % 6);   /* least significant digit   */
            if (prev_digit != -1)
            {
                if ((prev_digit == 2 && cd == 5) ||
                    (prev_digit == 5 && cd == 2))
                {
                    has_pair = 1;
                    break;
                }
            }
            prev_digit = cd;
            tmp = tmp / 6;              /* shift right one digit   */
        }

        if (has_pair && !last_has)
        {
            count++;
            if (count == n)
                return curr;
        }
        last_has = has_pair;
    }
}
