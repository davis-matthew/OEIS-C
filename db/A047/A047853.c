/*
 * A047853  a(n) = A047848(5, n).
 * https://oeis.org/A047853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^n + 6)/7 (equivalent recurrence a(0)=1,
 *            a(n)=8*a(n-1)-6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* apply the recurrence a(n)=8*a(n-1)-6 */
    {
        val = (val << 3) - 6;      /* shift left by 3 to multiply by 8, then subtract 6 */
    }
    return val;
}
