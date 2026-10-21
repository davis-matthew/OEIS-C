/*
 * A092498  Expansion of g.f. (1 + x + 2*x^2)/((1 - x)^3*(1 - x^3)).
 * https://oeis.org/A092498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 21*n^2 + 35*n + 18 - 6*floor((n+2)/3)) / 18
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    /* Compute floor((n+2)/3) using seq_t operations */
    seq_t floor_div = (nseq + (seq_t)2) / (seq_t)3;

    /* Numerator: 4*n^3 + 21*n^2 + 35*n + 18 - 6*floor_div */
    seq_t num = (seq_t)4 * nseq * nseq * nseq
              + (seq_t)21 * nseq * nseq
              + (seq_t)35 * nseq
              + (seq_t)18
              - (seq_t)6 * floor_div;

    /* Division by 18 */
    return num / (seq_t)18;
}
