/*
 * A158588  a(n) = 34*n^2 - 1.
 * https://oeis.org/A158588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for n < 1, return 0 as placeholder */
    }
    seq_t x = (seq_t)n;
    seq_t v = x * x;
    v = v * 34;
    v -= 1;
    return v;
}
