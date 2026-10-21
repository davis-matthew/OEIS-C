/*
 * A203629  Indices of 10-gonal (decagonal) numbers which are also 9-gonal
 *          (nonagonal).
 * https://oeis.org/A203629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 898*a(n-1) - a(n-2) - 336 a(1) = 1, a(2) = 551 (OEIS
 *            A203629)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)551;

    seq_t a1 = (seq_t)1;       /* a(1) */
    seq_t a2 = (seq_t)551;     /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t a3 = (seq_t)898 * a2 - a1 - (seq_t)336;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
