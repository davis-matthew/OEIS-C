/*
 * A071589  Numbers n such that Reversal(n) > n.
 * https://oeis.org/A071589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth positive integer such that reverse(n) > n
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i = 1;          /* current number to test */
    idx_t cnt = 0;        /* how many qualifying numbers we have seen */

    while (1) {
        seq_t x   = (seq_t)i;   /* work with seq_t type for consistency */
        seq_t rev = 0;
        seq_t t   = x;

        /* Compute the decimal reversal of x */
        while (t) {
            rev = rev * 10 + (t % 10);
            t   = t / 10;
        }

        /* Check the property */
        if (rev > x) {
            ++cnt;
            if (cnt == n)
                return x;
        }

        ++i;    /* move to the next integer */
    }
}
