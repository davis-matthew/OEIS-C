/*
 * A163609  a(n) = ((5 + 2*sqrt(2))*(3 + sqrt(2))^n + (5 - 2*sqrt(2))*(3 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2), a(0)=5, a(1)=19
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)19;
    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)19;
    seq_t an = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)6 - a0 * (seq_t)7;
        a0 = a1;
        a1 = an;
    }
    return an;
}
