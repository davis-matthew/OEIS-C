/*
 * A208951  Twice the maximum of the areas of the convex hulls of permutations
 *          {(1,p(1)), (2,p(2)), ..., (n,p(n))} of {1, 2, ..., n}, considered
 *          as points in the plane.
 * https://oeis.org/A208951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2n^2 - 8n + 10 for n >= 4, with a(1)=0, a(2)=0, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    if (n == 3) return 3;
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)2 * nn * nn - (seq_t)8 * nn + (seq_t)10;
    return val;
}
