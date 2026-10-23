/*
 * A157461  Expansion of x*(x+1) / (x^2-26*x+1).
 * https://oeis.org/A157461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 27, a(n) = 26*a(n-1) - a(n-2) for n > 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)27;

    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)27;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = (seq_t)26 * a2 - a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
