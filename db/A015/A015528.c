/*
 * A015528  a(n) = 3*a(n-1) + 10*a(n-2).
 * https://oeis.org/A015528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0ULL;
    if (n == 1)
        return 1ULL;

    seq_t a0 = 0ULL;          /* a(n-2) */
    seq_t a1 = 1ULL;          /* a(n-1) */
    seq_t an = 0ULL;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = 3 * a1 + 10 * a0;
        a0 = a1;
        a1 = an;
    }

    return an;
}
