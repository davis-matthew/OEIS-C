/*
 * A153127  a(n) = (2*n + 1)*(5*n + 6).
 * https://oeis.org/A153127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*(5*n + 6)
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t term1 = ns * 2 + 1;   /* 2*n + 1 */
    seq_t term2 = ns * 5 + 6;   /* 5*n + 6 */
    return term1 * term2;
}
