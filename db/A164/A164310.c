/*
 * A164310  a(n) = 6*a(n-1) - 6*a(n-2) for n > 1; a(0) = 4, a(1) = 15.
 * https://oeis.org/A164310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2), a(0) = 4, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)15;

    seq_t a0 = (seq_t)4;
    seq_t a1 = (seq_t)15;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t anew = (seq_t)((seq_t)6 * a1 - (seq_t)6 * a0);
        a0 = a1;
        a1 = anew;
    }
    return a1;
}
