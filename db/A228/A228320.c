/*
 * A228320  The Wiener index of the graph obtained by applying Mycielski's
 *          construction to the cycle graph C(n).
 * https://oeis.org/A228320
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*n - 13*n
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (sn * sn) * (seq_t)6 - sn * (seq_t)13;
}
