/*
 * A000165  Double factorial of even numbers: (2n)!! = 2^n*n!.
 * https://oeis.org/A000165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n * a(n-1), a(0) = 1 (i.e. a(n) = 2^n * n!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++)
        r *= (seq_t)2 * (seq_t)i;
    return r;
}
