/*
 * A031492  Numbers whose base-9 representation has the same number of 0's as
 *          8's.
 * https://oeis.org/A031492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑9 representation
 *            contains the same number of 0’s as of 8’s
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t cur = 1;
    idx_t count = 0;
    while (1) {
        seq_t tmp = cur;
        int c0 = 0, c8 = 0;
        while (tmp > 0) {
            int digit = (int)(tmp % (seq_t)9);
            if (digit == 0)
                c0++;
            else if (digit == 8)
                c8++;
            tmp /= 9;
        }
        if (c0 == c8) {
            count++;
            if (count == n)
                return cur;
        }
        cur++;
    }
}
