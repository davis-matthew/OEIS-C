/*
 * A052544  Expansion of (1-x)^2/(1 - 4*x + 3*x^2 - x^3).
 * https://oeis.org/A052544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3), a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)2;  /* a(n-2) */
    seq_t a2 = (seq_t)6;  /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (a2 << 2) - (a1 * (seq_t)3) + a0; /* 4*a2 - 3*a1 + a0 */
        a0 = a1;  /* shift the window */
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
