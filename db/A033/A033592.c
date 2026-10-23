/*
 * A033592  a(n) = (2*n+1)*(3*n+1)*(4*n+1)*(5*n+1).
 * https://oeis.org/A033592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(3*n+1)*(4*n+1)*(5*n+1)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 77, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 2*nn + 1;
    seq_t t2 = 3*nn + 1;
    seq_t t3 = 4*nn + 1;
    seq_t t4 = 5*nn + 1;
    return t1 * t2 * t3 * t4;
}
