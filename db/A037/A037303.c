/*
 * A037303  Numbers whose base-2 and base-5 expansions have the same digit
 *          sum.
 * https://oeis.org/A037303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer such that the sum of its binary
 *            digits equals the sum of its base‑5 digits
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n = 0 */

    idx_t found = 0;                  /* how many valid numbers found so far */

    for (seq_t x = 1; ; ++x)          /* iterate over all positive integers */
    {
        /* sum of binary digits (popcount) */
        idx_t sum_bin = 0;
        seq_t t = x;
        while (t != 0) {
            /* t & 1 is 1 when the lowest bit is set */
            if ((t & 1) != 0) /* if lowest bit is 1 */
                ++sum_bin;
            t >>= 1;          /* shift right by one bit */
        }

        /* sum of base‑5 digits */
        idx_t sum5 = 0;
        seq_t y = x;
        while (y != 0) {
            sum5 += (idx_t)(y % 5);     /* y % 5 yields a seq_t; cast to idx_t */
            y /= 5;
        }

        if (sum_bin == sum5) {          /* x satisfies the condition */
            ++found;
            if (found == n)
                return x;              /* nth such number found */
        }
    }
}
