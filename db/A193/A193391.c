/*
 * A193391  Wiener index of a benzenoid consisting of a spiral chain of n
 *          hexagons (s=1; see the Gutman et al. reference).
 * https://oeis.org/A193391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27 - 26*n + 72*n^2 + 8*n^3)/3
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 1045, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = ((seq_t)8) * n3 + ((seq_t)72) * n2 + (seq_t)27 - ((seq_t)26) * nn;
    return num / 3;
}
