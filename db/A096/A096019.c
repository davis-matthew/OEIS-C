/*
 * A096019  a(0)=3, a(n) = 3*a(n-1) + 2*(-1)^n.
 * https://oeis.org/A096019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*(-1)^n (a(0)=3, a(1)=7)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)3;
    if (n == (idx_t)1) return (seq_t)7;

    seq_t a0 = (seq_t)3;   /* a(0) */
    seq_t a1 = (seq_t)7;   /* a(1) */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = a1 * (seq_t)2 + a0 * (seq_t)3;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
