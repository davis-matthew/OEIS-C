/*
 * A162770  a(n) = ((2+sqrt(5))*(1+sqrt(5))^n + (2-sqrt(5))*(1-sqrt(5))^n)/2.
 * https://oeis.org/A162770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), a(0)=2, a(1)=7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)7;

    seq_t a0 = (seq_t)2;      /* a(0) */
    seq_t a1 = (seq_t)7;      /* a(1) */
    seq_t a2;                 /* a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        a2 = a1 * (seq_t)2 + a0 * (seq_t)4;
        a0 = a1;
        a1 = a2;
    }
    return a2;
}
