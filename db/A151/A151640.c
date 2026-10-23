/*
 * A151640  Number of permutations of 4 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^n - (4*n + 1)*5^n + 2*n*(4*n + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;
    const seq_t four = 4, two = 2, fifteen = 15, five = 5;

    seq_t pow15 = 1;
    seq_t pow5  = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow15 *= fifteen;
        pow5  *= five;
    }

    seq_t term1 = pow15;
    seq_t term2 = (four * sn + 1) * pow5;
    seq_t term3 = two * sn * (four * sn + 1);

    return term1 - term2 + term3;
}
