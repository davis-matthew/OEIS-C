/*
 * A074573  a(n) = 5^n + 6^n + 9^n.
 * https://oeis.org/A074573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 6^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p5 = 1;   /* 5^0 */
    seq_t p6 = 1;   /* 6^0 */
    seq_t p9 = 1;   /* 9^0 */

    for (idx_t i = 1; i <= n; i++) {
        p5 *= 5;
        p6 *= 6;
        p9 *= 9;
    }
    return p5 + p6 + p9;
}
