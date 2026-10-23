/*
 * A036543  a(n) = T(3,n), array T given by A048471.
 * https://oeis.org/A036543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6, a(0) = 1, a(1) = 9
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t prev = (seq_t)9;

    for (idx_t i = 2; i <= n; ++i)
    {
        prev = 3 * prev + 6;
    }

    return prev;
}
