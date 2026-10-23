/*
 * A162773  a(n) = ((2+sqrt(5))*(5+sqrt(5))^n + (2-sqrt(5))*(5-sqrt(5))^n)/2.
 * https://oeis.org/A162773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 20*a(n-2), a(0) = 2, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)15;
    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)15;
    seq_t an = a0;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)10 - a0 * (seq_t)20;
        a0 = a1;
        a1 = an;
    }
    return an;
}
