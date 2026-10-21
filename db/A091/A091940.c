/*
 * A091940  Given n colors, sequence gives number of ways to color the
 *          vertices of a square such that no edge has the same color on both
 *          of its vertices.
 * https://oeis.org/A091940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) + (n-1)^4 (number of proper vertex colourings of
 *            C4)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert to seq_t first */
    seq_t t = (seq_t)n - 1;        /* t = n - 1 */
    seq_t t2 = t * t;              /* t^2 */
    seq_t t4 = t2 * t2;            /* t^4 */
    return t + t4;                 /* (n-1) + (n-1)^4 */
}
