/*
 * A224134  Number of 3 X n 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8, a(n) = (4*n^3 + 15*n^2 + 35*n)/6 for n > 1
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)8;
    seq_t nn = (seq_t)n;
    /* compute 4*n^3 + 15*n^2 + 35*n using only integer arithmetic */
    seq_t num = 4*nn*nn*nn + 15*nn*nn + 35*nn;
    return num / 6;
}
