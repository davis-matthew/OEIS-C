/*
 * A042405  Denominators of continued fraction convergents to sqrt(730).
 * https://oeis.org/A042405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 54*a(n-1) + a(n-2), a(0)=1, a(1)=54
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 54;

    seq_t prevprev = 1;      /* a(0) */
    seq_t prev = 54;         /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i)
    {
        current = 54 * prev + prevprev;
        prevprev = prev;
        prev = current;
    }
    return prev;
}
