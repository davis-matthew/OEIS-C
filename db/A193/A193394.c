/*
 * A193394  Hyper-Wiener index of a benzenoid consisting of a zig-zag chain of
 *          n hexagons (s=13; see the Gutman et al. reference).
 * https://oeis.org/A193394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^4 + 24*n^3 + 28*n^2 + 147*n - 81)/3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 198, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Expect n >= 1; the formula is valid for all n >= 1 */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n2 * n2;
    seq_t num = 8 * n4 + 24 * n3 + 28 * n2 + 147 * N - 81;
    return num / 3;
}
