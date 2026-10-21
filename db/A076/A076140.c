/*
 * A076140  Triangular numbers T(k) that are three times another triangular
 *          number: T(k) such that T(k) = 3*T(m) for some m.
 * https://oeis.org/A076140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 3, a(0) = 0, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)3;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = (seq_t)14 * a1 - a0 + (seq_t)3;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
