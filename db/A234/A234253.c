/*
 * A234253  a(n) = Sum_{i=1..n} C(7+i,8)^2.
 * https://oeis.org/A234253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} C(7+i,8)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 51, u128 n <= 755,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;        /* C(8,8) for i = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        sum += bin * bin;
        if (i < n) {
            bin = bin * ((seq_t)(i + 8)) / ((seq_t)i);
        }
    }
    return sum;
}
