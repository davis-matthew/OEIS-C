/*
 * A099048  Number of 5 X n 0-1 matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (01;0), (10;0)
 *          and (11;0).
 * https://oeis.org/A099048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n + 14, a(1) = 32, a(2) = 50, a(3) = 68,
 * Range:     u8 n <= 13, u16 n <= 3640, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to result type */
    return sn * (seq_t)18 + (seq_t)14;
}
