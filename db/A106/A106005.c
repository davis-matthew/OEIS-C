/*
 * A106005  Fibonacci sequence (mod 15).
 * https://oeis.org/A106005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci number F(n) modulo 15
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t a0 = 0, a1 = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a0 + a1;
        a0 = a1;
        a1 = tmp % 15;
    }
    return a1;
}
