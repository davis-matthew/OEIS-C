/*
 * A083694  a(n) = 2*A002532(n).
 * https://oeis.org/A083694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t a0 = (seq_t)0;        /* a(n-2) */
    seq_t a1 = (seq_t)2;        /* a(n-1) */
    if (n == 1) return a1;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 2*a1 + 5*a0; /* a(n) */
        a0 = a1;
        a1 = next;
    }
    return a1;
}
