/*
 * A039068  Numbers whose base-8 representation has the same number of 3's and
 *          5's.
 * https://oeis.org/A039068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth nonnegative integer x such that in base‑8
 *            representation of x, the number of 3's equals the number of 5's
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but guard against n==0 */
    seq_t val = 0;
    idx_t found = 0;

    while (found < n) {
        seq_t tmp = val;
        unsigned count3 = 0, count5 = 0;

        while (tmp > 0) {
            seq_t digit = tmp % 8;
            if (digit == 3) ++count3;
            else if (digit == 5) ++count5;
            tmp = tmp / 8;
        }

        if (count3 == count5) {
            ++found;
            if (found == n)
                return val;
        }
        ++val;
    }
    return val; /* unreachable, but keeps the compiler happy */
}
