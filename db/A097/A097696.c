/*
 * A097696  Largest achievable determinant of a 4 X 4 matrix whose elements
 *          are the 16 consecutive integers n-15,...,n.
 * https://oeis.org/A097696
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2400*(2*n-15) for n>=11, a(8)=7343, a(9)=8784,
 *            a(10)=12065, a(n)=a(15-n) for n<8 (behaviour below offset not
 *            required)
 * Range:     u8 none, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8) /* below offset – undefined; return 0 */
        return 0;

    if (n == 8u)
        return (seq_t)7343;
    if (n == 9u)
        return (seq_t)8784;
    if (n == 10u)
        return (seq_t)12065;

    /* n >= 11 */
    seq_t n_t = (seq_t)n;
    return (seq_t)2400 * ((seq_t)2 * n_t - (seq_t)15);
}
