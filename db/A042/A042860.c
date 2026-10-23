/*
 * A042860  Numerators of continued fraction convergents to sqrt(962).
 * https://oeis.org/A042860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 62*a(n-1) + a(n-2), a(0) = 31, a(1) = 1923
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)31;
    if (n == 1) return (seq_t)1923;
    seq_t prev2 = (seq_t)31;
    seq_t prev1 = (seq_t)1923;
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)62 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
