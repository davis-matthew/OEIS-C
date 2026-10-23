/*
 * A037423  Positive numbers having the same set of digits in base 4 and base
 *          5.
 * https://oeis.org/A037423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose set of digits in base 4
 *            equals the set of digits in base 5 (digit 4 is not allowed)
 * Range:     u8 n <= 14, u16 n <= 5365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;               /* candidate integer */
    seq_t found = 0;           /* how many valid numbers found so far */

    while (1) {
        /* compute digit set for base 4 */
        uint8_t mask4 = 0;
        seq_t t = x;
        while (t > 0) {
            int d = (int)(t % 4);
            mask4 |= (1U << d);
            t = t / 4;
        }

        /* compute digit set for base 5 */
        uint8_t mask5 = 0;
        t = x;
        while (t > 0) {
            int d = (int)(t % 5);
            mask5 |= (1U << d);
            t = t / 5;
        }

        /* accept if the two digit sets are identical (no 4 in base 5) */
        if (mask4 == mask5) {
            ++found;
            if (found == n)
                return x;
        }

        ++x;
    }
}
