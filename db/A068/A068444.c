/*
 * A068444  a(0) = 10; for n>0, a(n) = n*a(n-1)-n-2.
 * https://oeis.org/A068444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * a(n-1) - n - 2 , with a(0) = 10
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = 10;                /* a(0) */
    if (n == 0) return prev;

    for (idx_t i = 1; i <= n; ++i)
    {
        prev = (seq_t)i * prev - (seq_t)i - 2;
    }
    return prev;
}
