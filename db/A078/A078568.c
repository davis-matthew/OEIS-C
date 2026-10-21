/*
 * A078568  Repunits concatenated with A000422, where A000422 is the
 *          concatenation of numbers from n down to 1.
 * https://oeis.org/A078568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = repunit(n) concatenated with the concatenation of the *
 *            numbers n, n-1, …, 1. * For n = 1: 1 | 1 → 11 * For n = 2: 11 |
 *            21 → 1121 * For n = 3: 111 | 321 → 111321 * ... * where
 *            repunit(n) is the integer consisting of n digits ‘1’
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* append the n ones of the repunit */
    for (idx_t i = 0; i < n; ++i)
        res = res * 10 + 1;

    /* append the numbers n, n-1, …, 1 */
    for (idx_t k = n; k > 0; --k) {
        idx_t x = k;
        /* store the decimal digits of x in reverse order */
        unsigned char rev[20];   /* n ≤ 10,000 → at most 5 digits, 20 is safe */
        int len = 0;
        if (x == 0) {
            rev[len++] = 0;
        } else {
            while (x > 0) {
                rev[len++] = x % 10;
                x /= 10;
            }
        }
        /* reverse the digits to obtain the normal order */
        for (int d = len - 1; d >= 0; --d)
            res = res * 10 + rev[d];
    }

    return res;
}
