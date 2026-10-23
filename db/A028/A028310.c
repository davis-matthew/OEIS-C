/*
 * A028310  Expansion of (1 - x + x^2) / (1 - x)^2 in powers of x.
 * https://oeis.org/A028310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(0) = 1, a(n) = n for n > 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    return (seq_t)n;
}
