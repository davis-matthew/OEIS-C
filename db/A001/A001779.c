/*
 * A001779  Expansion of 1/((1+x)(1-x)^8).
 * https://oeis.org/A001779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} A001753-like iterated partial sums: a =
 *            7-fold partial sums of b(n) = (1+(-1)^n)/2, i.e. G.f.
 *            1/((1+x)(1-x)^8) = (1/((1-x)(1+x))) * 1/(1-x)^7
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 84, u64 n <= 2105,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s[8];
    int k;
    idx_t i;
    for (k = 0; k < 8; k++) s[k] = 0;
    for (i = 0; i <= n; i++) {
        s[0] = (i & 1) ? 0 : 1;
        for (k = 1; k < 8; k++) s[k] += s[k - 1];
    }
    return s[7];
}
