/*
 * A213842  Principal diagonal of the convolution array A213841.
 * https://oeis.org/A213842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 15*n^2 + 16*n^3)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 683, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t num = 2*nn - 15*nn2 + 16*nn3;
    return num / 3;
}
