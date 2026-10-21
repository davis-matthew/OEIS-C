/*
 * A216110  The Wiener index of the meta-polyphenyl chain with n hexagons (see
 *          the Dou et al. and the Deng references).
 * https://oeis.org/A216110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^3 + 18*n^2 - 9*n
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return 18 * n3 + 18 * n2 - 9 * nn;
}
