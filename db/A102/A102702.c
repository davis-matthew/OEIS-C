/*
 * A102702  Expansion of (2-x-2*x^2-x^3)/(1-x-x^2)^2.
 * https://oeis.org/A102702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 41, u64 n <= 86, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+a(n-2)-2*a(n-3)-a(n-4),
   with a(0)=2, a(1)=3, a(2)=6, a(3)=10 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)10;
    seq_t a0 = (seq_t)2, a1 = (seq_t)3, a2 = (seq_t)6, a3 = (seq_t)10;
    seq_t an = 0;
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)2 * a3 + a2;
        an -= (seq_t)2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
