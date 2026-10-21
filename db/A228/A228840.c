/*
 * A228840  a(n) = 3^n*A228569(n).
 * https://oeis.org/A228840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2 a(1) = 15 a(n) = 15*a(n-1) - 27*a(n-2) for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)15;

    seq_t a0 = (seq_t)2, a1 = (seq_t)15;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 15 * a1 - 27 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
