/*
 * A159678  The general form of the recurrences are the a(j), b(j) and n(j)
 *          solutions of the 2-equation problem 7*n(j) + 1 = a(j)*a(j) and
 *          9*n(j) + 1 = b(j)*b(j) with positive integer numbers.
 * https://oeis.org/A159678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 17 and for n ≥ 3, a(n) = 16*a(n-1) - a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)17;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)17;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t a = (seq_t)16 * a2 - a1;
        a1 = a2;
        a2 = a;
    }
    return a2;
}
