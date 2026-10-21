/*
 * A068722  Number of solenoidal flows (flow in = flow out) in a 3 X 3 square
 *          array with integer velocities -n .. n.
 * https://oeis.org/A068722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+2*n+2*n^2) * (1+3*n+3*n^2)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 163, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 1 + 2*nn + 2*nn*nn;
    seq_t t2 = 1 + 3*nn + 3*nn*nn;
    return t1 * t2;
}
