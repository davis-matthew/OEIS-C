/*
 * A034977  Expansion of 1/(1-64*x)^(1/8), related to octo-factorial numbers
 *          A045755.
 * https://oeis.org/A034977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n * A045755(n) / n!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t val = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* a(k) = a(k-1) * 8*(8*k-7) / k  */
        seq_t num = (seq_t)8 * ((seq_t)8 * k - (seq_t)7);
        val = (val * num) / k;
    }
    return val;
}
