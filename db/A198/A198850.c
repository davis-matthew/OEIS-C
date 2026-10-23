/*
 * A198850  a(n) = 2*8^n - 1.
 * https://oeis.org/A198850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*8^n - 1 Recurrence: a(0)=1; a(n)=8*a(n-1)+7 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * 8 + 7;  /* a(i+1) = 8*a(i) + 7 */
    }
    return result;
}
