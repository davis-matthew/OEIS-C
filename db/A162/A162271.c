/*
 * A162271  a(n) = ((5+sqrt(2))*(4+sqrt(2))^n + (5-sqrt(2))*(4-sqrt(2))^n)/2.
 * https://oeis.org/A162271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((5+sqrt(2))*(4+sqrt(2))^n + (5-sqrt(2))*(4-sqrt(2))^n)/2
 *            * Recurrence: a(0)=5, a(1)=22, a(n)=8*a(n-1)-14*a(n-2) (n>1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)5;
    if (n == 1u)
        return (seq_t)22;

    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)22;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)8 * a1 - (seq_t)14 * a0;
        a0 = a1;
        a1 = cur;
    }
    return a1;
}
