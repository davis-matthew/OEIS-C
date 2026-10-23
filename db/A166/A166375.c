/*
 * A166375  a(n) = sum (floor (j^2/n)) taken over 1 <= j <= n-1.
 * https://oeis.org/A166375
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1}^{n-1} floor(j^2 / n)
 * Range:     u8 n <= 16, u16 n <= 335, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j < n; ++j) {
        seq_t square = ((seq_t)j) * ((seq_t)j);
        seq_t term   = square / (seq_t)n;   /* floor division */
        sum += term;
    }
    return sum;
}
