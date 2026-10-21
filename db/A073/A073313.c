/*
 * A073313  Binomial transform of generalized Lucas numbers S(n) = S(n-1) +
 *          S(n-2) + S(n-3), S(0)=3, S(1)=1, S(2)=3.
 * https://oeis.org/A073313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3); a(0)=3, a(1)=4, a(2)=8
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    seq_t a0 = (seq_t)3; /* a(n-3) */
    seq_t a1 = (seq_t)4; /* a(n-2) */
    seq_t a2 = (seq_t)8; /* a(n-1) */
    seq_t aN = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        aN = (seq_t)4 * a2 - (seq_t)4 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = aN;
    }
    return aN;
}
