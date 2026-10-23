/*
 * A034663  Sum of n-th powers of divisors of 21.
 * https://oeis.org/A034663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (1+3^n)*(1+7^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p3 = (seq_t)1;
    seq_t p7 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        p3 *= (seq_t)3;
        p7 *= (seq_t)7;
    }
    return (p3 + (seq_t)1) * (p7 + (seq_t)1);
}
