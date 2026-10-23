/*
 * A123109  a(0) = 1, a(1) = 3, a(n) = 3*a(n-1) + 3 for n > 1.
 * https://oeis.org/A123109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 3*a(n-1) + 3 for n > 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t val = (seq_t)3;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * (seq_t)3 + (seq_t)3;   /* a(i) = 3*a(i-1)+3 */
    }
    return val;
}
