/*
 * A205342  Number of length 5 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements unequal but differing by no more
 *          than n.
 * https://oeis.org/A205342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n^3 - 3*n^2 + 7*n)/6
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */
    seq_t ns = (seq_t)n;           /* cast to seq_t for calculations */
    seq_t n2 = ns * ns;            /* n^2 */
    seq_t n3 = n2 * ns;            /* n^3 */
    seq_t num = 8 * n3 - 3 * n2 + 7 * ns;
    return num / 6;                /* numerator is always divisible by 6 */
}
