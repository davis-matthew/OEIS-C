/*
 * A212032  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any element at a city block distance of two, and containing the
 *          value n(n+1)/2-3.
 * https://oeis.org/A212032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n⁸ + 12*n⁷ – 106*n⁶ – 24*n⁵ + 1635*n⁴ – 4516*n³ +
 *            3652*n² + 2800*n – 3840) / 384 for n > 1; a(1) = 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sn = (seq_t)n;
    seq_t n1 = sn;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * sn;
    seq_t n6 = n3 * n3;
    seq_t n7 = n6 * sn;
    seq_t n8 = n4 * n4;

    seq_t num = (seq_t)0;
    num += (seq_t)3   * n8;
    num += (seq_t)12  * n7;
    num -= (seq_t)106 * n6;
    num -= (seq_t)24  * n5;
    num += (seq_t)1635 * n4;
    num -= (seq_t)4516 * n3;
    num += (seq_t)3652 * n2;
    num += (seq_t)2800 * n1;
    num -= (seq_t)3840;

    return num / (seq_t)384;
}
