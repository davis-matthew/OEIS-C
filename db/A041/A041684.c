/*
 * A041684  Numerators of continued fraction convergents to sqrt(362).
 * https://oeis.org/A041684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 19, a(1) = 723; for n > 1, a(n) = 38*a(n-1) + a(n-2)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;
    if (n == 1)
        return (seq_t)723;

    seq_t prevprev = 19;   /* a(0) */
    seq_t prev     = 723;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 38 * prev + prevprev;
        prevprev = prev;
        prev     = cur;
    }

    return prev;
}
