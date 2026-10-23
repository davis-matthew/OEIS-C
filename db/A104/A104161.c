/*
 * A104161  G.f.: x*(1 - x + x^2)/((1-x)^2 * (1 - x - x^2)).
 * https://oeis.org/A104161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + n-1; a(0) = 0, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;

    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 + a0 + (seq_t)(i - 1);
        a0 = a1;
        a1 = an;
    }
    return a1;
}
