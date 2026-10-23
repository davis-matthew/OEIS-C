/*
 * A181879  Expansion of x*(1+x)/(1-3*x-4*x^2-x^3).
 * https://oeis.org/A181879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3) (n ≥ 3) a(0) = 0, a(1) = 1,
 *            a(2) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)4;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = a2 * (seq_t)3 + a1 * (seq_t)4 + a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
