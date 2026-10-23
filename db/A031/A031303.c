/*
 * A031303  Position of n-th 4 in A031298.
 * https://oeis.org/A031303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 4 in the concatenation of * the
 *            decimal representations of the natural numbers written in *
 *            reverse order
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pos = 0;        /* current position in the generated string   */
    idx_t count = 0;      /* number of 4's encountered so far          */

    for (idx_t i = 1; ; ++i) {
        idx_t j = i;
        /* Write i reversed, digit by digit */
        while (j > 0) {
            int digit = j % 10;
            ++pos;                         /* advance to next digit          */
            if (digit == 4) {
                ++count;
                if (count == n)            /* found the n-th 4              */
                    return pos;
            }
            j /= 10;                       /* move to next digit of i        */
        }
    }
}
