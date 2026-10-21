/*
 * A074610  a(n) = 3^n + 9^n.
 * https://oeis.org/A074610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == (idx_t)0)
        return (seq_t)2;
    if (n == (idx_t)1)
        return (seq_t)12;

    /* Compute 3^n and 9^n by repeated multiplication. */
    seq_t p3 = (seq_t)1;
    seq_t p9 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        p3 *= (seq_t)3;
        p9 *= (seq_t)9;
    }
    return p3 + p9;
}
