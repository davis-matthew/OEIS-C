/*
 * A087944  Expansion of (1-4*x+3*x^2)/((1-2*x)*(1-4*x+x^2)).
 * https://oeis.org/A087944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n + (2+sqrt(3))^n + (2-sqrt(3))^n) / 3 Initial values:
 *            a(0)=1, a(1)=2, a(2)=6 Recurrence for n>2: a(n) = 6*a(n-1) -
 *            9*a(n-2) + 2*a(n-3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
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

    seq_t a0 = (seq_t)1, a1 = (seq_t)2, a2 = (seq_t)6;
    seq_t an = 0;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t temp = a2 * (seq_t)6;
        temp += a0 * (seq_t)2;
        seq_t sub = a1 * (seq_t)9;
        an = temp - sub;          /* safe: temp >= sub for all n ≥ 3 */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
