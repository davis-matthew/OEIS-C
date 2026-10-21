/*
 * A099945  Number of 4 X n binary matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (00;1) and (11;0).
 * https://oeis.org/A099945
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*2^n - 28
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* undefined for indices below the offset */
    seq_t pow2 = (seq_t)1 << n;
    seq_t result = (seq_t)27 * pow2 - (seq_t)28;
    return result;
}
