/*
 * A099003  Number of 4 X n 0-1 matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1), (10;0) and
 *          (11;0).
 * https://oeis.org/A099003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*2^n - 14
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* start with 2^0 = 1 */
    for (idx_t i = 0; i < n; ++i)  /* compute 2^n */
        val <<= 1;                 /* val = val * 2 */
    val *= 15;                     /* multiply by 15 */
    val -= 14;                     /* subtract 14 */
    return val;
}
