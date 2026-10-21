/*
 * A074621  a(n) = 6^n + 9^n.
 * https://oeis.org/A074621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n + 9^n, offset 0 (a(0)=2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 6^n and 9^n by repeated multiplication */
    seq_t pow6 = 1;
    seq_t pow9 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow6 *= (seq_t)6;
        pow9 *= (seq_t)9;
    }
    return pow6 + pow9;
}
