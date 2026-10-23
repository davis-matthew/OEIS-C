/*
 * A045258  Numbers whose base-5 representation contains exactly two 1's and
 *          three 4's.
 * https://oeis.org/A045258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑5 representation contains exactly two 1's and three 4's. */
/* The function scans natural numbers in order, checks the base‑5 digit counts,
   and returns the n-th such number (a(1) is the first). */
/* It is valid for any seq_t and idx_t as defined by the build. */
seq_t a(idx_t n)
{
    if (n == 0U) /* offset starts at 1; undefined for n==0 */
        return 0;

    seq_t num = 1U;      /* candidate number */
    idx_t  found = 0U;   /* how many satisfying numbers have been found */

    for (;; ++num)
    {
        /* Count digits 1 and 4 in the base‑5 representation of num. */
        int count1 = 0;
        int count4 = 0;
        seq_t temp = num;

        while (temp != 0)
        {
            unsigned digit = (unsigned)(temp % 5U);
            if (digit == 1U)
                ++count1;
            else if (digit == 4U)
                ++count4;
            temp /= 5U;
        }

        /* If the digit counts match the required 2 ones and 3 fours,
           increment found and return when we hit the target index. */
        if (count1 == 2 && count4 == 3)
        {
            ++found;
            if (found == n)
                return num;
        }
    }
}
