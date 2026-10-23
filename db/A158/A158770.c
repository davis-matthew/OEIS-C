/*
 * A158770  a(n) = 1521*n^2 - 39.
 * https://oeis.org/A158770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1521*n^2 - 39 (offset 1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 1680, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the value is undefined for this offset; return 0. */
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t nn = (seq_t)n;
    return (seq_t)1521 * nn * nn - (seq_t)39;
}
