/*
 * A052683  Expansion of e.g.f. 2*x^4/(1-x).
 * https://oeis.org/A052683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 4; * a(4) = 48; * a(n) = n * a(n-1) for n > 4 *
 *            (equivalently a(n) = 2 * n! for n > 3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        return (seq_t)0;
    }
    if (n == 4) {
        return (seq_t)48;
    }
    seq_t result = (seq_t)48;          /* a(4) */
    for (idx_t i = 5; i <= n; ++i) {
        result = result * (seq_t)i;    /* n * a(n-1) recurrence */
    }
    return result;
}
