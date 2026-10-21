/*
 * A217585  Number of triangles with endpoints of the form (x,x^2), x in
 *          {-n,...,n}, having at least one angle of 45 degrees.
 * https://oeis.org/A217585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n-1 for n > 2 a(0) = 0, a(1) = 1, a(2) = 5
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    /* n > 2 */
    seq_t res = (seq_t)4 * (seq_t)n;
    res -= (seq_t)1;
    return res;
}
