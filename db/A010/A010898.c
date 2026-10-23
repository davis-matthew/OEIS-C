/*
 * A010898  Minimal scope of a (5,n) difference triangle.
 * https://oeis.org/A010898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = (n^3 + 9 n^2 + 26 n - 6)/6
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t val = (n3 + 9 * n2 + 26 * nn - 6) / 6;
    return val;
}
