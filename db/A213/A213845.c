/*
 * A213845  Principal diagonal of the convolution array A213844.
 * https://oeis.org/A213845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 9*n^2 + 16*n^3)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 680, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t to avoid any implicit narrowing */
    seq_t nn = (seq_t)n;

    /* compute numerator: 16*n^3 - 9*n^2 + 2*n   */
    /* the value is always divisible by 3, so integer division is exact */
    seq_t num = 16 * nn * nn * nn - 9 * nn * nn + 2 * nn;

    return num / 3;
}
