/*
 * A085409  Sum of three solutions of the Diophantine equation x^2 - y^2 =
 *          z^3.
 * https://oeis.org/A085409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^3 + 3*n^2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 781, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t nsq = ns * ns;
    seq_t ncu = nsq * ns;
    return 9 * ncu + 3 * nsq;
}
