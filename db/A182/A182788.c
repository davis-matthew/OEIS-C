/*
 * A182788  Number of n-colorings of the 3 X 3 X 3 triangular grid.
 * https://oeis.org/A182788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)^4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) {
        return (seq_t)0;
    }
    /* Cast n to seq_t once, then do all arithmetic in seq_t */
    seq_t a = (seq_t)n;
    seq_t b = a - (seq_t)1;
    seq_t c = a - (seq_t)2;
    seq_t d = c * c;     /* (n-2)^2 */
    seq_t e = d * d;     /* (n-2)^4 */
    return a * b * e;
}
