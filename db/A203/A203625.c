/*
 * A203625  Indices of octagonal numbers which are also decagonal.
 * https://oeis.org/A203625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 195*a(n-1) - 195*a(n-2) + a(n-3) with a(1) = 1, a(2) =
 *            135, a(3) = 26125
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)135;
    if (n == 3) return (seq_t)26125;

    seq_t a1 = (seq_t)1;          /* a(n-3) */
    seq_t a2 = (seq_t)135;        /* a(n-2) */
    seq_t a3 = (seq_t)26125;      /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t temp = (seq_t)195 * a3 - (seq_t)195 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = temp;
    }
    return a3;
}
