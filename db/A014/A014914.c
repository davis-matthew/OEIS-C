/*
 * A014914  a(1)=1, a(n) = 25*a(n-1) + n.
 * https://oeis.org/A014914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=25*a(n-1)+n for n >= 2 (OEIS A014914)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    seq_t a_prev = (seq_t)1; /* a(1) */
    if (n == 1)
        return a_prev;
    for (idx_t i = 2; i <= n; ++i)
        a_prev = a_prev * (seq_t)25 + (seq_t)i;
    return a_prev;
}
