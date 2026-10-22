/*
 * A000071  a(n) = Fibonacci(n) - 1.
 * https://oeis.org/A000071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, hear
 * Formula:   a(n) = Fibonacci(n) - 1, with F(0) = 0, F(1) = 1, F(n) = F(n-1)
 *            + F(n-2)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f0 = 0, f1 = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    /* f0 now holds F(n) */
    return f0 - 1;
}
