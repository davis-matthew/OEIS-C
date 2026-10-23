/*
 * A197985  a(n) = round((n+1/n)^2).
 * https://oeis.org/A197985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^2 + 2, for n > 1; a(1) = 4
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)4;
    }
    seq_t x = (seq_t)n;
    return x * x + (seq_t)2;
}
