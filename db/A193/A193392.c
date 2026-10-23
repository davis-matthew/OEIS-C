/*
 * A193392  Hyper-Wiener index of a benzenoid consisting of a spiral chain of
 *          n hexagons (s=1; see the Gutman et al. reference).
 * https://oeis.org/A193392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 + 28*n^3 + 154*n^2 - 169*n + 111)/3
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t num = 2 * n4 + 28 * n3 + 154 * n2 - 169 * nn + 111;
    return num / 3;
}
