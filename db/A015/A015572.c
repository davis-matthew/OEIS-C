/*
 * A015572  Expansion of x/(1 - 7*x - 12*x^2).
 * https://oeis.org/A015572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 7*a(n-1) + 12*a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (seq_t)7 * a1 + (seq_t)12 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
