/*
 * A084136  Binomial transform of cosh(sqrt(2)*x)^2.
 * https://oeis.org/A084136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) - 7*a(n-3), a(0)=1, a(1)=1, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)5;   /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i)
    {
        ai = (seq_t)3 * a2 + (seq_t)5 * a1 - (seq_t)7 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
