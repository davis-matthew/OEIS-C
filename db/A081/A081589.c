/*
 * A081589  Third row of Pascal-(1,5,1) array A081580.
 * https://oeis.org/A081589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*n^2 - 6*n + 1
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;
    seq_t res = (seq_t)18 * n_sq;
    res -= (seq_t)6 * sn;
    res += (seq_t)1;
    return res;
}
