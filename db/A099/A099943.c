/*
 * A099943  Number of 5 X n binary matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (01,1) and
 *          (11;0).
 * https://oeis.org/A099943
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*n + 20 (for n >= 2)
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    return (seq_t)n * 26 + 20;
}
