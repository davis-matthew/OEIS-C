/*
 * A042404  Numerators of continued fraction convergents to sqrt(730).
 * https://oeis.org/A042404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 54*a(n-1)+a(n-2) (a(0)=27, a(1)=1459)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)27;
    }
    if (n == 1) {
        return (seq_t)1459;
    }

    seq_t prev2 = (seq_t)27;   /* a(n-2) */
    seq_t prev1 = (seq_t)1459; /* a(n-1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 * (seq_t)54 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
