/*
 * A193395  Wiener index of a benzenoid consisting of a double-step zig-zag
 *          chain of n hexagons (n >= 2, s = 2123; see the Gutman et al.
 *          reference).
 * https://oeis.org/A193395
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n^3 + 24*n^2 + 74*n + 6*(-1)^n - 51)/3
 * Range:     u8 none, u16 n <= 15, u32 n <= 928, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = 16 * nseq * nseq * nseq
               + 24 * nseq * nseq
               + 74 * nseq;
    if ((n & 1) == 0)
        term += 6;
    else
        term -= 6;
    term -= 51;
    return term / 3;
}
