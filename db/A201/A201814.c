/*
 * A201814  Number of arrays of 6 integers in -n..n with sum zero and equal
 *          numbers of elements greater than zero and less than zero.
 * https://oeis.org/A201814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*n^5 + 65*n^3 + 64*n + 1 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 52, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* promote index to seq_t */
    seq_t n2 = ns * ns;           /* n^2 */
    seq_t n3 = n2 * ns;           /* n^3 */
    seq_t n5 = n3 * n2;           /* n^5 = n^3 * n^2 */
    return (seq_t)11 * n5 + (seq_t)65 * n3 + (seq_t)64 * ns + (seq_t)1;
}
