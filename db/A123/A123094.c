/*
 * A123094  Sum of first n 12th powers.
 * https://oeis.org/A123094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n (1^12 + 2^12 + ... + n^12)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 36, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow = (seq_t)i;
        /* compute i^12 by repeated multiplication (12 multiplications total) */
        for (int j = 1; j < 12; ++j)
            pow *= (seq_t)i;
        sum += pow;
    }
    return sum;
}
