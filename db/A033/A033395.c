/*
 * A033395  a(n) = floor(75/n).
 * https://oeis.org/A033395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(75/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed ≥1; for safety n==0 returns 0 */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)75 / (seq_t)n;
}
