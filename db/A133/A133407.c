/*
 * A133407  a(n) = a(n-1) + 5*a(n-2) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A133407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-1)+5*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t a_prevprev = (seq_t)1; /* a(0) */
    seq_t a_prev     = (seq_t)2; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = a_prev + a_prevprev * (seq_t)5;
        a_prevprev = a_prev;
        a_prev = cur;
    }
    return a_prev;
}
