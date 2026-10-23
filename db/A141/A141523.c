/*
 * A141523  Expansion of (3-2*x-3*x^2)/(1-x-x^2-x^3).
 * https://oeis.org/A141523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=1, a(2)=1, a(n)=a(n-1)+a(n-2)+a(n-3) for n≥3
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    seq_t a0 = (seq_t)3;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = a0 + a1 + a2;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
