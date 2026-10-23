/*
 * A015583  a(0) = 0, a(1) = 1; for n >= 2, a(n) = 9*a(n-1) + 7*a(n-2).
 * https://oeis.org/A015583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1; for n >= 2, a(n) = 9*a(n-1) + 7*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t a0 = 0;
    seq_t a1 = 1;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = 9 * a1 + 7 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
