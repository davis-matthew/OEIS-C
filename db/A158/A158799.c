/*
 * A158799  a(0)=1, a(1)=2, a(n)=3 for n >= 2.
 * https://oeis.org/A158799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(n+1, 3)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)(n + 1);  /* n = 0 -> 1, n = 1 -> 2 */
    else
        return (seq_t)3;        /* n >= 2 -> 3 */
}
