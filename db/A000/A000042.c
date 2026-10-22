/*
 * A000042  Unary representation of natural numbers.
 * https://oeis.org/A000042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n) = 10*a(n-1) + 1, a(1) = 1 (i.e. a(n) = (10^n - 1)/9)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 10 + 1;
    return r;
}
