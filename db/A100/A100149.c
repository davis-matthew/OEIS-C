/*
 * A100149  Structured small rhombicubeoctahedral numbers.
 * https://oeis.org/A100149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(37*n^3 - 45*n^2 + 14*n)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 850, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nt  = (seq_t)n;
    seq_t nt2 = nt * nt;          /* n^2 */
    seq_t nt3 = nt2 * nt;         /* n^3 */
    /* Compute 37*n^3 + 14*n - 45*n^2  before dividing by 6 to avoid unsigned underflow. */
    seq_t value = ((seq_t)37 * nt3 + (seq_t)14 * nt - (seq_t)45 * nt2) / 6;
    return value;
}
